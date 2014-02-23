/*
 * HistogramContainer.h
 *
 *  Created on: 13 Sep 2011
 *      Author: coopers
 */

#ifndef HISTOGRAMCONTAINER_H_
#define HISTOGRAMCONTAINER_H_
#include <vector>
#include <string>
#include "TH1.h"
#include "TH2.h"
#include "TString.h"

class HistogramContainer {
private:
	std::string             m_strMsgTag;
	std::vector<TH1D*>* 	m_1dHistos;
	std::vector<TH2D*>* 	m_2dHistos;
	typedef 				std::vector<TH1D*>::iterator iterator1D;
	typedef 				std::vector<TH2D*>::iterator iterator2D;


public:
	HistogramContainer();
	HistogramContainer(std::string name);
	virtual ~HistogramContainer();

	void add1DHisto(TH1D* histo);
	void add2DHisto(TH2D* histo);
	TH1D* find1DHisto(TString name);
	TH2D* find2DHisto(TString name);

	void write1DHistos();
	void write2DHistos();

};

#endif /* HISTOGRAMCONTAINER_H_ */
