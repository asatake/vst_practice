#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#include "audioeffectx.h"


#define MY_VST_VENDOR       "test vendor"
#define MY_VST_VERSION      1
#define MY_VST_PRODUCT_NAME "Sample Tremolo VST"
#define MY_VST_NAME         "Tremolo"

#define MY_VST_INPUT_NUM   2
#define MY_VST_OUTPUT_NUM  2

#define MY_VST_UNIQUE_ID  'SMPL'

#define MY_VST_PRESET_NUM    1
#define MY_VST_PARAMETER_NUM 2

#define TREMOLO_SPEED 0
#define TREMOLO_DEPTH 1

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_NO_WARNINGS

class MyTremoloVST : public AudioEffectX
{
protected:
	float fTime;
	float fTremoloSpeed;
	float fTremoloDepth;

public:
	MyTremoloVST (audioMasterCallback audioMaster);
	~MyTremoloVST ();

	virtual bool getVendorString (char* text);
	virtual VstInt32 getVendorVersion ();
	virtual bool getProductString (char* text);
	virtual bool getEffectName (char* name);

	virtual void  setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void  getParameterName (VstInt32 index, char* text);
	virtual void  getParameterLabel (VstInt32 index, char* label);
	virtual void  getParameterDisplay (VstInt32 index, char* text);

	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
};

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new MyTremoloVST (audioMaster);
}

MyTremoloVST::MyTremoloVST (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, MY_VST_PRESET_NUM, MY_VST_PARAMETER_NUM)
{

	setNumInputs (MY_VST_INPUT_NUM);
	setNumOutputs (MY_VST_OUTPUT_NUM);
	setUniqueID (MY_VST_UNIQUE_ID);

	isSynth (false);

	canProcessReplacing ();

	fTime = 0.0f;

	fTremoloSpeed = 1.0f / 4.0f;
	fTremoloDepth = 0.5f;
}

MyTremoloVST::~MyTremoloVST ()
{

}

bool MyTremoloVST::getVendorString (char* text)
{
	vst_strncpy (text, MY_VST_VENDOR, kVstMaxVendorStrLen);
	return true;
}

VstInt32 MyTremoloVST::getVendorVersion ()
{
	return MY_VST_VERSION;
}

bool MyTremoloVST::getProductString (char* text)
{
	vst_strncpy (text, MY_VST_PRODUCT_NAME, kVstMaxProductStrLen);
	return true;
}

bool MyTremoloVST::getEffectName (char* name)
{
	vst_strncpy (name, MY_VST_NAME, kVstMaxEffectNameLen);
	return true;
}

void  MyTremoloVST::setParameter (VstInt32 index, float value)
{
	switch (index)
	{
	case TREMOLO_SPEED:
		fTremoloSpeed = 1.0f / (29.0f*value +1.0f);
		break;
	case TREMOLO_DEPTH:
		fTremoloDepth = value;
		break;
	}
}

float MyTremoloVST::getParameter (VstInt32 index)
{
	float value = 0.0f;
	switch (index)
	{
	case TREMOLO_SPEED:
		value = (1 / fTremoloSpeed - 1.0f) / 29.0f;
		break;
	case TREMOLO_DEPTH:
		value = fTremoloDepth;
		break;
	}
	return value;
}

void  MyTremoloVST::getParameterName (VstInt32 index, char* text)
{
	switch (index)
	{
	case TREMOLO_SPEED:
		vst_strncpy (text, "Speed", kVstMaxParamStrLen);
		break;
	case TREMOLO_DEPTH:
		vst_strncpy (text, "Depth", kVstMaxParamStrLen);
		break;
	}
}

void  MyTremoloVST::getParameterLabel (VstInt32 index, char* label)
{
	switch (index)
	{
	case TREMOLO_SPEED:
		vst_strncpy (label, "Hz", kVstMaxParamStrLen);
		break;
	case TREMOLO_DEPTH:
		vst_strncpy (label, "dB", kVstMaxParamStrLen);
		break;
	}
}

void  MyTremoloVST::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index)
	{
	case TREMOLO_SPEED:
		float2string (1.0f / fTremoloSpeed, text, kVstMaxParamStrLen);
		break;
	case TREMOLO_DEPTH:
		float2string (fTremoloDepth, text, kVstMaxParamStrLen);
		break;
	}
}

void MyTremoloVST::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* inL  =  inputs[0];
	float* inR  =  inputs[1];
	float* outL = outputs[0];
	float* outR = outputs[1];

	float dt = 1.0f / getSampleRate ();
	float pi = 3.14159265f;

	while (sampleFrames > 0)
	{
		fTime = fTime + dt;
		if (fTime > fTremoloSpeed)
		{
			fTime -= fTremoloSpeed;
		}

		(*outL) = (*inL) * ( 1.0f - fTremoloDepth * (0.5f + 0.5f*sin(2.0f*pi*fTime/fTremoloSpeed)));
		(*outR) = (*inR) * ( 1.0f - fTremoloDepth * (0.5f + 0.5f*sin(2.0f*pi*fTime/fTremoloSpeed)));

		inL++;
		inR++;
		outL++;
		outR++;
		sampleFrames--;
	}
}
