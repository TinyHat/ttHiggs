/*
 * HistogramContainer.C
 *
 *  Created on: 13 Sep 2011
 *      Author: coopers
 */

#include "SMTnTupAnalysis/HistogramContainer.h"
#include <iostream>

HistogramContainer::HistogramContainer() {
	m_1dHistos = new std::vector<TH1D*>();
	m_2dHistos = new std::vector<TH2D*>();
	m_strMsgTag = "";
}

HistogramContainer::HistogramContainer(std::string name) {
	m_1dHistos = new std::vector<TH1D*>();
	m_2dHistos = new std::vector<TH2D*>();
	m_strMsgTag = name;
}

HistogramContainer::~HistogramContainer() {
  for (iterator1D iter = m_1dHistos->begin(); iter < m_1dHistos->end(); iter++)
  {
    delete (*iter);
  }
  for (iterator2D iter = m_2dHistos->begin(); iter < m_2dHistos->end(); iter++)
  {
    delete (*iter);
  }
  m_1dHistos->clear();
  m_2dHistos->clear();
  delete m_1dHistos;
  delete m_2dHistos;
}


void HistogramContainer::add1DHisto(TH1D* histo) {
	m_1dHistos->push_back(histo);
}

void HistogramContainer::add2DHisto(TH2D* histo) {
	m_2dHistos->push_back(histo);
}

TH1D* HistogramContainer::find1DHisto(TString name){

	for (iterator1D iter = m_1dHistos->begin(); iter < m_1dHistos->end(); iter++)
	{
		if ( (*iter)->GetName() == name)
		{
			return (*iter);
		}
	}
	std::cout << m_strMsgTag << "HistogramContainer::find1DHisto():: ERROR Cannot find histogram with name: " << name << std::endl;
	std::cout << m_strMsgTag << "HistogramContainer::find1DHisto():: ERROR Null pointer returned" << name << std::endl;
	return NULL;
}

TH2D* HistogramContainer::find2DHisto(TString name){

	for (iterator2D iter = m_2dHistos->begin(); iter < m_2dHistos->end(); iter++)
	{
		if ( (*iter)->GetName() == name)
		{
			return (*iter);
		}
	}
	std::cout << m_strMsgTag << "HistogramContainer::find2DHisto():: ERROR Cannot find histogram with name: " << name << " Please check spelling" << std::endl;
	std::cout << m_strMsgTag << "HistogramContainer::find2DHisto():: ERROR Null pointer returned" <<std::endl;
	return NULL;
}

void HistogramContainer::write1DHistos()
{
	for (iterator1D iter = m_1dHistos->begin(); iter < m_1dHistos->end(); iter++)
	{
		(*iter)->Write();
	}
}

void HistogramContainer::write2DHistos()
{
	for (iterator2D iter = m_2dHistos->begin(); iter < m_2dHistos->end(); iter++)
	{
		(*iter)->Write();
	}
}
