//
//  arkParamsPanel.h
//  arkGUI
//
//  Created by anthonycouret on 17/02/2015.
//  Copyright (c) 2015 ac. All rights reserved.
//

#ifndef __arkGUI__arkParamsPanel__
#define __arkGUI__arkParamsPanel__

#include <memory>
#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include "arkMediatorWidget.h"

class arkParamsPanel;
typedef std::shared_ptr<arkParamsPanel> arkParamsPanelShPtr;
typedef std::weak_ptr<arkParamsPanel> arkParamsPanelWkPtr;

class arkParamsPanel : public QWidget, public arkMediatorWidget
{
    Q_OBJECT
    
    public :
    
    static arkParamsPanelShPtr create( arkAbstractMediatorShPtr mediator_shptr );
    ~arkParamsPanel();
    
    private :
    
    arkParamsPanel( arkAbstractMediatorShPtr mediator_shptr );
    arkParamsPanelWkPtr m_weak_ptr;
    
    QGridLayout * m_layout;
    
    QSpinBox * m_tesselation_factor;
    QSpinBox * m_nbSamples;
    QSpinBox * m_disp_factor;
    
    QComboBox * m_leadr_mode;
    QComboBox * m_normal_mode;
    
    public slots :
    
    void setLEADRMode(int);
    void setNormalMode(int);
    void updateTesselationFactor(int);
    void updateNbSamples(int);
    void updateDispFactor(int);
    
};

#endif /* defined(__arkGUI__arkParamsPanel__) */
