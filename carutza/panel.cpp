/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
--------------------------------------------------------------------------------

 This file is part of DASHSASH dashboard

    CARUTZA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CARUTZA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CARUTZA.  If not, see <http://www.gnu.org/licenses/>.
*/
/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

#include <QDir>
#include <QObject>
#include "panel.h"
#include "menu.h"
#include "theapp.h"
#include "client.h"
#include "mysett.h"
#include "panel.h"
#include "imagez.h"
#include "appman.h"
#include "qslunchbutt.h"


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
Panel::Panel(CfgPanel* pe, QWidget *parent)
    : CtrlHolder(pe, parent),_inotify(0),
      _moved(false),
      _overflow(0),
      _selected(-1),
      _hidden(false),
      _folderchanged(false)

{


    _panimation = new QPropertyAnimation(this, "geometry");
    if(pe->_notify) _inotify = new DskWatch(this);
    connect(_panimation,SIGNAL(finished()),this,SLOT(slot_anim_done()));


    _basefolder = _pcfg->_name;
    _curfolder = _basefolder;
    slot_change_folder();

}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
Panel::~Panel(void)
{
    delete _inotify;
    delete _panimation;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::_config_ui(const std::vector<XwnSet>& buts)
{
    Q_UNUSED(buts);
    int dr = CFG(_drect).x();
    Q_UNUSED(dr);
    int x = _pcfg->_rect.width(),y=_pcfg->_rect.height();
    this->resize(x, y);
    x = _pcfg->_rect.left(),y=_pcfg->_rect.top();
    this->move(x,y);
    this->show();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
size_t  Panel::_load_buts(const QString& folder,
                          const QPoint& sz,
                          std::vector<XwnSet>& buts)
{
    QString         fld;
    if(::access((const char*)folder,0)==0)
        fld = folder;
    else {
        fld = CFG(_workdir+folder);
    }
    QDir            dir(fld);
    QFileInfoList   files = dir.entryInfoList();
    int             icwidth = 0;

    foreach(const QFileInfo &fi, files)
    {
        if(fi.fileName().at(0)=='.')
            continue;
        QString     sfile = fi.absoluteFilePath();

        if(sfile.endsWith(".desktop") || sfile.endsWith(".desktopp"))
        {
            MySett df(sfile);
            XwnSet butset;
            if(df.ok() && butset.load_config(df))
            {
                butset._desk_file = sfile;
                butset._wt = XwnSet::WIG_DESKTOP;
                cond_if(butset._imgdim.x()<=0,butset._imgdim = sz);
                cond_if(butset._align==0,butset._align = _pcfg->_align);
                icwidth+=butset._imgdim.x()+_pcfg->_spacing;
                buts.push_back(butset);
            }
        }
        else if(sfile.endsWith(".applet") )
        {
            MySett df(sfile);
            XwnSet butset;
            if(df.ok() && butset.load_config(df))
            {
                butset._desk_file = sfile;
                butset._wt = XwnSet::WIG_APPLET;
                cond_if(butset._imgdim.x()<=0, butset._imgdim = sz);
                cond_if(butset._align==0,butset._align = _pcfg->_align);
                icwidth+=butset._imgdim.x()+_pcfg->_spacing;
                buts.push_back(butset);
            }
        }
        else if(sfile.endsWith(".signal") ) // internal function signal call
        {
            MySett df(sfile);
            XwnSet butset;
            if(df.ok() && butset.load_config(df))
            {
                butset._desk_file = sfile;
                butset._wt = XwnSet::WIG_SIGNAL;
                cond_if(butset._imgdim.x()<=0, butset._imgdim = sz);
                cond_if(butset._align==0,butset._align = _pcfg->_align);
                icwidth+=butset._imgdim.x()+_pcfg->_spacing;
                buts.push_back(butset);
            }
        }else if(fi.isDir() && !fi.isHidden())
        {
            QString fn = sfile+"/desktop.conf";
            if(0==::access(fn.toUtf8(),0))
            {
                MySett df(fn);

                XwnSet butset;
                butset._desk_file = sfile;
                butset.load_config(df);
                butset._pname=">";
                butset._cmd=sfile; //which folder to change
                cond_if(butset._imgdim.x()<=0, butset._imgdim = sz);
                cond_if(butset._align==-1,butset._align = _pcfg->_align);
                icwidth+=butset._imgdim.x()+_pcfg->_spacing;
                buts.push_back(butset);
            }
        }
    }
    cond_if(folder != _basefolder, icwidth += sz.x()+1); /// add back buton space
    return icwidth;
}


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::fit_to_parent(const QSize& proom)
{
    //this->size(proom);
    this->setFixedWidth(proom.width());
    this->setFixedHeight(proom.width());
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::_load_controls(const QString& folder, std::vector<XwnSet>& buts)
{
    QPoint              sz = _pcfg->_icons;
    int                 swidth=_parent_width();

    Q_UNUSED(swidth);
    cond_if(sz.x()<=0, sz= QPoint(_pcfg->_rect.width(),
                                  _pcfg->_rect.height()));
    _icwidth = sz.x();
    std::vector<QWidget*>::iterator b = _butons.begin();
    for(;b!= _butons.end();++b)
    {
        delete *b;
    }
    _butons.clear();
    _layerwidth = _load_buts(folder, sz, buts);
    this->move(_pcfg->_rect.left(),_pcfg->_rect.top());

    _layout_it(buts.size()+int(folder != _basefolder), _pcfg->_spacing, _pcfg->_rect);
    _layout->setAlignment((Qt::AlignmentFlag)(_pcfg->_align));


    if(folder != _basefolder)
    {
        XwnSet set;
        set._imgdim.setX(96);
        set._imgdim.setY(96);
        set._wt =  XwnSet::WIG_BACK;
        set._cmd = _basefolder;
        set._name = " ";
        LunchButt* pb = new LunchButt(this, sz, set);
        pb->set_image(CFG(_base).toUtf8(), "app_back.png");
        connect(pb,SIGNAL(sig_clicked(OdButton*)),this,
                SLOT(change_folder(OdButton*)));
        connect(pb,SIGNAL(sig_scrolled(int)),this,
                SLOT(done_scrolling(int)));
        connect(pb,SIGNAL(sig_scrolled(int)),this,
                SLOT(done_scrolling(int)));
        _add_widget(pb,set._imgdim, 0,0,Qt::AlignLeft);
        _butons.push_back(pb);
    }

    std::vector<XwnSet>::iterator it = buts.begin();

    int row = 0;
    int col = folder != _basefolder;
    for(;it!= buts.end();++it)
    {
        _add_button((*it), col, row);
        ++col;
    }

    if(_inotify)
    {
        _inotify->watch(folder);
    }
    _re_stretch();
    _calc_overflow();
    _selected=-1;
}

/*--------------------------------------------------------------------------------------
  messy, but there were no requirements and whatever talks changed every day
  -------------------------------------------------------------------------------------*/
void Panel::_add_button(const XwnSet& xs, int col, int row)
{
    OdButton* pb;
    if(xs._wt == XwnSet::WIG_DESKTOP)
    {
        pb = new LunchButt(this, xs._imgdim, xs);
        MySett& ps = MySett::config();
        QString qsi = ps.images();

        pb->set_image(qsi,xs._icon);
        cond_if(!xs._caticon.isEmpty(),pb->set_cat_image("",xs._caticon));
        connect(pb,SIGNAL(sig_moving(OdButton*,int,int)),this,SLOT(scrool_lancer(OdButton*,int,int)));
        connect(pb,SIGNAL(sig_scrolled(int)),this,SLOT(done_scrolling(int)));
        if(xs._pname==">") // sub folder
            connect(pb,SIGNAL(sig_clicked(OdButton*)),this,SLOT(change_folder(OdButton*)));
        else
            connect(pb,SIGNAL(sig_clicked(OdButton*)),this,SLOT(run_app(OdButton*)));
        _add_widget(pb, xs._imgdim, col, row, xs._align);
    }
    else if(xs._wt == XwnSet::WIG_SIGNAL)
    {
        if(xs._cmd == "Appman")
        {
            pb = new Appman(this, xs._imgdim, xs);
            pb->set_image("",xs._icon);
            cond_if(!xs._caticon.isEmpty(),pb->set_cat_image("",xs._caticon));
            connect(pb,SIGNAL(sig_moving(OdButton*,int,int)),this,SLOT(scrool_lancer(OdButton*,int,int)));
            connect(pb,SIGNAL(sig_scrolled(int)),this,SLOT(done_scrolling(int)));
            PA->set_appman((Appman*)pb);
        }
        //connect(pb,SIGNAL(sig_clicked(LunchButt*)),this,SLOT(slot_pressed(LunchButt*)));
        else
        {
            pb = new LunchButt(this, xs._imgdim, xs);
            pb->set_image("",xs._icon);
            cond_if(!xs._caticon.isEmpty(),pb->set_cat_image("",xs._caticon));
            connect(pb,SIGNAL(sig_moving(OdButton*,int,int)),this,SLOT(scrool_lancer(OdButton*,int,int)));
            connect(pb,SIGNAL(sig_scrolled(int)),this,SLOT(done_scrolling(int)));
            if(xs._pname==">") // sub folder
                connect(pb,SIGNAL(sig_clicked(OdButton*)),this,SLOT(change_folder(OdButton*)));
            else
                connect(pb,SIGNAL(sig_clicked(OdButton*)),this,SLOT(run_app(OdButton*)));
        }
        _add_widget(pb, xs._imgdim, col, row, xs._align);
    }
    else
    {
        pb = new QsLunchButt(this, xs._imgdim, xs);
        connect(pb,SIGNAL(sig_clicked(OdButton*)),this,SLOT(run_intern(OdButton*)));
        pb->set_image("",xs._icon);
        connect(pb,SIGNAL(sig_moving(OdButton*,int,int)),this,SLOT(scrool_lancer(OdButton*,int,int)));
        connect(pb,SIGNAL(sig_scrolled(int)),this,SLOT(done_scrolling(int)));

        _add_widget(pb, xs._imgdim, col, row, xs._align);
    }
    _butons.push_back(pb);
    pb->show();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::slot_pressed(OdButton* pb)
{
    if(pb->xset()._cmd=="kill")
    {
        emit PA->appman()->on_click();
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::change_folder(OdButton* pb)
{
    if(pb)
    {
        _folderchanged=true;
        usleep(1000);
        QString newfolder = pb->xset()._cmd;
        if(!newfolder.isEmpty())
        {
            _curfolder = newfolder;
            QTimer::singleShot(100, this, SLOT(slot_change_folder()));
        }
        usleep(0xFFFF);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::slot_change_folder()
{
    if(!_curfolder.isEmpty())
    {
        std::vector<XwnSet> buts;
        _load_controls(_curfolder, buts);
        _config_ui(buts);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::slot_floder_ch(const QString& folder)
{
    if(_curfolder == folder)
    {
        QTimer::singleShot(100, this, SLOT(slot_change_folder()));
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::run_app(OdButton* pb)
{
    _folderchanged=false;
    CtrlHolder::run_app(pb);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::run_intern(OdButton* pb)
{
    PA->runintern(pb->xset());
}


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::scrool_lancer(OdButton* lb, int dx ,int dy)
{
    Q_UNUSED(lb);
    _scrool_lancer(dx,dy);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::done_scrolling(int cx)
{
    _done_scrooling(cx);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::mousePressEvent(QMouseEvent *event)
{
    _presspt = event->pos();
    _abtomove = true;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::mouseMoveEvent(QMouseEvent * event)
{
    if(_abtomove &&
            ((abs(_presspt.x()-event->pos().x()) > 16) ||
             (abs(_presspt.y()-event->pos().y()) > 16)))
    {
        _scrool_lancer(event->pos().x()-_presspt.x(),
                       event->pos().y()-_presspt.y());
        qDebug()<<event->pos().x()-_presspt.x();
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::mouseReleaseEvent(QMouseEvent *event)
{
    int cx = this->pos().x() - event->pos().x();
    _done_scrooling(cx);
    _selected=-1;
    _update_btnsel();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::_done_scrooling(int dr)
{
    Q_UNUSED(dr);
    int  swidth= _parent_width();

    //this->setDisabled(true);
    cond_if(_icwidth==0,_icwidth=128);
    _abtomove = false;
    int xs = this->pos().x();
    if(_moved && xs)
    {
        int maxleft = this->width() - swidth;
        int xe = 0;
        if(xs > 0)
        {
            xe = 0;
        }
        else if(xs < maxleft)
        {
            xe=-maxleft;
        }
        else
        {
            int mox = abs(xs % _icwidth) > (_icwidth/2);
            xe  = (xs / _icwidth) - mox;
            xe *= _icwidth;
        }
        /*
        _panimation->setDuration(300);
        _panimation->setStartValue(QRect(xs,this->pos().y(),
                                         this->rect().width(),this->rect().height()));

        _panimation->setEndValue(QRect(xe,this->pos().y(),
                                       this->rect().width(),this->rect().height()));
        _panimation->setEasingCurve(QEasingCurve::OutCubic);
        _panimation->start();
*/

    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::_scrool_lancer(int dx ,int dy)
{
    Q_UNUSED(dy);
    int    swidth=CFG(_drect).x()-64;

    QPoint pt = this->pos();
    int newx = pt.x()+dx;
    if(newx>0)
        newx=0;
    else if (newx < -(this->width() - swidth))
        newx =- ((this->width() - swidth));
    move(newx, _pcfg->_rect.y());
    _moved=true;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::slot_align_edge()
{
    //int   animx = this->pos().x();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::_calc_overflow()
{
    int swidth= _parent_width();
    _overflow=0;
    if(_layerwidth > swidth)
    {
        int pos = this->pos().x();
        if(pos < 0)
            _overflow|=0x1;
        if(pos + this->rect().width() > swidth)
            _overflow|=0x2;
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
    //process_key(event->key());
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::_update_btnsel()
{
    int swidth= _parent_width();

    std::vector<QWidget*>::iterator it = _butons.begin();
    for(;it!= _butons.end();++it)
    {
        LunchButt* pb = dynamic_cast<LunchButt*>(*it);
        if(pb)
            pb->deactivate();
    }
    cond_if(_selected==-1, return);

    QWidget* pb = _butons[_selected];

    int bx = pb->pos().x();
    int tx = this->pos().x();
    int onleft = tx+bx;
    _panimation->setStartValue(QRect(tx,this->pos().y(),this->rect().width(),this->rect().height()));
    _panimation->setDuration(500);
    _panimation->setEasingCurve(QEasingCurve::OutCubic);
    if(onleft<0)
    {
        int xe = -onleft;
        _panimation->setEndValue(QRect(xe,this->pos().y(),this->rect().width(),this->rect().height()));
        return;
    }

    int onright = tx+bx;
    if(onright > swidth)
    {
        onright = swidth-(onright+pb->rect().width());
        int xe = -(tx-onright);
        _panimation->setEndValue(QRect(xe,this->pos().y(),this->rect().width(),this->rect().height()));

    }
    _panimation->start();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void   Panel::refresh_buts()
{
    std::vector<QWidget*>::iterator b = _butons.begin();
    for(;b!= _butons.end();++b)
    {
        LunchButt* pb = dynamic_cast<LunchButt*>(*b);
        if(pb)
            pb->refresh();
    }
    //this->move(_pcfg->_rect.left(),_pcfg->_rect.top());
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void     Panel::ahide(bool hide)
{
    Q_UNUSED(hide);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::slot_anim_done()
{
    if(_moved)
        _calc_overflow();
    this->refresh_buts();
    _moved=false;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
int Panel::_parent_width()
{
    QObject* pp = parent();
    if(pp)
    {
        QWidget* pqw = dynamic_cast<QWidget*>(pp);
        if(pqw)
            return pqw->rect().width();
    }
    return CFG(_drect.y());
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Panel::update_widget(const QString& name, const QString& value)
{
    OdButton* pb = this->get_widget(name);
    if(pb)
    {
        pb->setText(value);
        pb->update();
    }
}
