#include "SetPositionIssue.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

SetPositionIssue::SetPositionIssue(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPrograms))
{
  GetParam(kGain)->InitDouble("Gain", 0., 0., 100.0, 0.01, "%");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, 1.);
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds();
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 20; ++j)
      {
        IRECT r;
        r.L = 30 + 100*(float)i;
        r.R = r.L + 100;
        r.T = 30 + 20*(float)j;
        r.B = r.T + 30;
        pGraphics->AttachControl(new ITextControl(r, "Hello iPlug 2!", IText(15)));
      }
    }

    mPosY = 0;

    IRECT r;
    r.L = 30;
    r.R = r.L + 500;
    r.T = mPosY;
    r.B = r.T + 30;
    mButton = new IVButtonControl(r);
    pGraphics->AttachControl(mButton);
  };
#endif
}

void SetPositionIssue::OnIdle()
{
  mButton->SetPosition(30,mPosY);

  mPosY += 30;
  if (mPosY > 600)
  {
    mPosY = 0;
  }

  ////////////////////////////////////////////////
  // UNCOMMENT THIS LINE TO REPRODUCE THE ISSUE
  ////////////////////////////////////////////////
  GetUI()->SetAllControlsDirty();
}


#if IPLUG_DSP
void SetPositionIssue::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
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
