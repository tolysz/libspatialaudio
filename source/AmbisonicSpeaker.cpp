/*############################################################################*/
/*#                                                                          #*/
/*#  Ambisonic C++ Library                                                   #*/
/*#  CAmbisonicSpeaker - Ambisonic Speaker                                   #*/
/*#  Copyright � 2007 Aristotel Digenis                                      #*/
/*#  Copyright © 2017 Videolabs                                              #*/
/*#                                                                          #*/
/*#  Filename:      AmbisonicSpeaker.cpp                                     #*/
/*#  Version:       0.2                                                      #*/
/*#  Date:          19/05/2007                                               #*/
/*#  Author(s):     Aristotel Digenis, Peter Stitt                           #*/
/*#  Licence:       LGPL (+ Proprietary)                                     #*/
/*#                                                                          #*/
/*############################################################################*/


#include "AmbisonicSpeaker.h"


CAmbisonicSpeaker::CAmbisonicSpeaker()
{
    Create(DEFAULT_ORDER, DEFAULT_HEIGHT, 0);
	Refresh();
}

CAmbisonicSpeaker::~CAmbisonicSpeaker()
{

}

bool CAmbisonicSpeaker::Create(AmbUInt nOrder, AmbBool b3D, AmbUInt nMisc)
{
    bool success = CAmbisonicSource::Create(nOrder, b3D, nMisc);
    if(!success)
        return false;
    //SetOrderWeight(0, sqrtf(2.f)); // This seems to break SN3D weighting
    
    return true;
}

void CAmbisonicSpeaker::Refresh()
{
	CAmbisonicSource::Refresh();
}

void CAmbisonicSpeaker::Process(CBFormat* pBFSrc, AmbUInt nSamples, AmbFloat* pfDst)
{
	AmbUInt niChannel = 0;
	AmbUInt niSample = 0;
	memset(pfDst, 0, nSamples * sizeof(AmbFloat));
	for(niChannel = 0; niChannel < m_nChannelCount; niChannel++)
	{
		if(m_b3D){
			for(niSample = 0; niSample < nSamples; niSample++)
			{
				// The spherical harmonic coefficients are multiplied by (2*order + 1) to provide the correct decoder
				// for SN3D normalised Ambisonic inputs.
				pfDst[niSample] += pBFSrc->m_ppfChannels[niChannel][niSample]
							* m_pfCoeff[niChannel] * (2*floor(sqrt(niChannel)) + 1);
			}
		}
		else
		{
			for(niSample = 0; niSample < nSamples; niSample++)
			{
				// The spherical harmonic coefficients are multiplied by 2 to provide the correct decoder
				// for SN3D normalised Ambisonic inputs decoded to a horizontal loudspeaker array
				pfDst[niSample] += pBFSrc->m_ppfChannels[niChannel][niSample]
							* m_pfCoeff[niChannel] * 2.f;
			}
		}
	}
}
