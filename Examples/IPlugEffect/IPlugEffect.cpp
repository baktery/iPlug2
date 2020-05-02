#include "IPlugEffect.h"
#include "IPlug_include_in_plug_src.h"


IPlugEffect::IPlugEffect(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPrograms))
, mControlTrackName(nullptr)
{
  GetParam(kGain)->InitDouble("Gain", 0., 0., 100.0, 0.01, "%");

#if IPLUG_EDITOR // All UI methods and member variables should be within an IPLUG_EDITOR guard, should you want distributed UI
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, 1.);
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds();
    //pGraphics->AttachControl(new ITextControl(b.GetMidVPadded(50), "Hello iPlug 2!", IText(50)));
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(100).GetVShifted(-100), kGain));

    mControlTrackName = new ITextControl(IRECT(0,0,200,50), "", IText(50));
    pGraphics->AttachControl(mControlTrackName);

    mControlTrackIndex = new ITextControl(IRECT(0,50,200,100), "", IText(50));
    pGraphics->AttachControl(mControlTrackIndex);

    mControlTrackNamespace = new ITextControl(IRECT(0,100,200,150), "", IText(50));
    pGraphics->AttachControl(mControlTrackNamespace);

    mControlTrackNamespaceIndex = new ITextControl(IRECT(0,150,200,200), "", IText(50));
    pGraphics->AttachControl(mControlTrackNamespaceIndex);
  };
#endif
}

void IPlugEffect::OnIdle()
{
  if (GetUI())
  {
    WDL_String trackName;
    GetTrackName(trackName);
    mControlTrackName->SetStr("force_reset_for_text_color");
    mControlTrackName->SetStr(trackName.Get());

    IText text;
    GetTrackColor(text.mFGColor.R,text.mFGColor.G,text.mFGColor.B);
    mControlTrackName->SetText(text);

    char str[256];
    sprintf(str,"%d",GetTrackIndex());
    mControlTrackIndex->SetStr(str);

    WDL_String trackNamespace;
    GetTrackNamespace(trackNamespace);
    mControlTrackNamespace->SetStr(trackNamespace.Get());

    sprintf(str,"%d",GetTrackNamespaceIndex());
    mControlTrackNamespaceIndex->SetStr(str);
  }

}

#if IPLUG_DSP
void IPlugEffect::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const double gain = GetParam(kGain)->Value() / 100.;
  const int nChans = NOutChansConnected();
  
  for (int s = 0; s < nFrames; s++) {
    for (int c = 0; c < nChans; c++) {
      outputs[c][s] = inputs[c][s] * gain;
    }
  }
}
#endif
