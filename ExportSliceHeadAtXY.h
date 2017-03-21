#pragma once

// Plugin implementation class
class CExportSliceHeadAtXY
{
public:
  CExportSliceHeadAtXY(IfmDocument pDoc);
  ~CExportSliceHeadAtXY();
  static CExportSliceHeadAtXY* FromHandle(IfmDocument pDoc);

#pragma region IFM_Definitions
  // Implementation
public:
  void OnActivate (IfmDocument pDoc, Widget wParent);
  void PostTimeStep (IfmDocument pDoc);
  void PostFlowSimulation (IfmDocument pDoc);

#pragma endregion

private:
  IfmDocument m_pDoc;
public: 
	int CExportSliceHeadAtXY::ExportSliceHeadAtXY(IfmDocument pDoc);
	int iTransientFileHeadSwitch;
};
