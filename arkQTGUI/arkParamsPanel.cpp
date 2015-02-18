//
//  arkParamsPanel.cpp
//  arkGUI
//
//  Created by anthonycouret on 17/02/2015.
//  Copyright (c) 2015 ac. All rights reserved.
//

#include "arkParamsPanel.h"

arkParamsPanelShPtr arkParamsPanel::create( arkAbstractMediatorShPtr mediator_shptr )
{
    arkParamsPanelShPtr arkParamsPanel_sh_ptr( new arkParamsPanel( mediator_shptr ) );
    arkParamsPanel_sh_ptr->m_weak_ptr = arkParamsPanel_sh_ptr;
    return arkParamsPanel_sh_ptr;
}

arkParamsPanel::arkParamsPanel( arkAbstractMediatorShPtr mediator_shptr ) :
QWidget(),
arkMediatorWidget( mediator_shptr )
{
    m_layout = new QGridLayout();
    setLayout( m_layout );
    
    m_tesselation_factor = new QSpinBox();
    m_disp_factor = new QSpinBox();
    m_nbSamples = new QSpinBox();
    m_disp_factor->setValue(0);

    m_leadr_mode = new QComboBox();
    m_leadr_mode->addItem( tr("Enabled") );
    m_leadr_mode->addItem( tr("Disabled") );
    
    m_normal_mode = new QComboBox();
    m_normal_mode->addItem( tr("Enabled") );
    m_normal_mode->addItem( tr("Disabled") );

    m_layout->addWidget( new QLabel( tr("tesselation factor : ") ), 0, 0, 2, 8 );
    m_layout->addWidget( m_tesselation_factor, 0, 8, 2, 2 );
    m_tesselation_factor->setValue(1);
    m_tesselation_factor->setMinimum(1);

    m_layout->addWidget( new QLabel( tr("displacement factor : ") ), 2, 0, 2, 8 );
    m_layout->addWidget( m_disp_factor, 2, 8, 2, 2 );
    
    m_layout->addWidget( new QLabel( tr("nb samples : ") ), 4, 0, 2, 8 );
    m_layout->addWidget( m_nbSamples, 4, 8, 2, 2 );
    m_nbSamples->setValue(1);
    m_nbSamples->setMinimum(1);
    m_nbSamples->setMaximum(256);
    
    m_layout->addWidget( new QLabel( tr("LEADR mode : ") ), 6, 0, 2, 6 );
    m_layout->addWidget( m_leadr_mode, 6, 6, 2, 4 );

    m_layout->addWidget( new QLabel( tr("LEADR mode : ") ), 8, 0, 2, 6 );
    m_layout->addWidget( m_normal_mode, 8, 6, 2, 4 );
    
    connect
    ( m_tesselation_factor, SIGNAL( valueChanged(int) ), this, SLOT( updateTesselationFactor(int) ) );
    
    connect( m_disp_factor, SIGNAL( valueChanged(int) ), this, SLOT( updateDispFactor(int) ) );
    
    connect( m_nbSamples, SIGNAL( valueChanged(int) ), this, SLOT( updateNbSamples(int) ) );
    
    connect( m_leadr_mode, SIGNAL( currentIndexChanged(int) ), this, SLOT( setLEADRMode(int) ) );
    
    connect( m_normal_mode, SIGNAL( currentIndexChanged(int) ), this, SLOT( setNormalMode(int) ) );
}

arkParamsPanel::~arkParamsPanel()
{
    
}

void arkParamsPanel::setLEADRMode( int index )
{
    m_mediator_shptr->setLEADRMode( index == 0 ? true : false );
}

void arkParamsPanel::setNormalMode( int index )
{
    m_mediator_shptr->setNormalMode( index == 0 ? true : false );
}

void arkParamsPanel::updateTesselationFactor(int value)
{
    m_mediator_shptr->setTesselationFactor( value );
}

void arkParamsPanel::updateNbSamples( int value )
{
    m_mediator_shptr->setNbSamples( value );
}

void arkParamsPanel::updateDispFactor( int value )
{
    m_mediator_shptr->setDispFactor( value );
}
