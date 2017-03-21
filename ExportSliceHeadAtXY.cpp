#include "stdifm.h"
#include "ExportSliceHeadAtXY.h"
#include <fstream>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <ifm/module.h>
#include <ifm/graphic.h>
#include <ifm/document.h>
#include <ifm/archive.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <direct.h> 
#include <math.h>
#include <iomanip>
#include <sstream>
#include <direct.h> 
#include <string>
#include <cmath>
#include <string>


IfmModule g_pMod;  /* Global handle related to this plugin */

#pragma region IFM_Definitions
/* --- IFMREG_BEGIN --- */
/*  -- Do not edit! --  */

static IfmResult OnBeginDocument (IfmDocument);
static void OnEndDocument (IfmDocument);
static void OnActivate (IfmDocument, Widget);
static void PostTimeStep (IfmDocument);
static void PostFlowSimulation (IfmDocument);

/*
 * Enter a short description between the quotation marks in the following lines:
 */
static const char szDesc[] = 
  "Please, insert a plug-in description here!";

#ifdef __cplusplus
extern "C"
#endif /* __cplusplus */

IfmResult RegisterModule(IfmModule pMod)
{
  if (IfmGetFeflowVersion (pMod) < IFM_REQUIRED_VERSION)
    return False;
  g_pMod = pMod;
  IfmRegisterModule (pMod, "SIMULATION", "EXPORTSLICEHEADATXY", "ExportSliceHeadAtXY", 0x1000);
  IfmSetDescriptionString (pMod, szDesc);
  IfmSetCopyrightPath (pMod, "ExportSliceHeadAtXY.txt");
  IfmSetHtmlPage (pMod, "ExportSliceHeadAtXY.htm");
  IfmSetPrimarySource (pMod, "ExportSliceHeadAtXY.cpp");
  IfmRegisterProc (pMod, "OnBeginDocument", 1, (IfmProc)OnBeginDocument);
  IfmRegisterProc (pMod, "OnEndDocument", 1, (IfmProc)OnEndDocument);
  IfmRegisterProc (pMod, "OnActivate", 1, (IfmProc)OnActivate);
  IfmRegisterProc (pMod, "PostTimeStep", 1, (IfmProc)PostTimeStep);
  IfmRegisterProc (pMod, "PostFlowSimulation", 1, (IfmProc)PostFlowSimulation);
  return True;
}

static void OnActivate (IfmDocument pDoc, Widget button)
{
  CExportSliceHeadAtXY::FromHandle(pDoc)->OnActivate (pDoc, button);
}
static void PostTimeStep (IfmDocument pDoc)
{
  CExportSliceHeadAtXY::FromHandle(pDoc)->PostTimeStep (pDoc);
}
static void PostFlowSimulation (IfmDocument pDoc)
{
  CExportSliceHeadAtXY::FromHandle(pDoc)->PostFlowSimulation (pDoc);
}

/* --- IFMREG_END --- */
#pragma endregion


static IfmResult OnBeginDocument (IfmDocument pDoc)
{
  if (IfmDocumentVersion (pDoc) < IFM_CURRENT_DOCUMENT_VERSION)
    return false;

  try {
    IfmDocumentSetUserData(pDoc, new CExportSliceHeadAtXY(pDoc));
  }
  catch (...) {
    return false;
  }

  return true;
}

static void OnEndDocument (IfmDocument pDoc)
{
  delete CExportSliceHeadAtXY::FromHandle(pDoc);
}

///////////////////////////////////////////////////////////////////////////
// Implementation of CExportSliceHeadAtXY

// Constructor
CExportSliceHeadAtXY::CExportSliceHeadAtXY (IfmDocument pDoc)
  : m_pDoc(pDoc)
	, iTransientFileHeadSwitch(0)
{
  /*
   * TODO: Add your own code here ...
   */
}

// Destructor
CExportSliceHeadAtXY::~CExportSliceHeadAtXY ()
{
  /*
   * TODO: Add your own code here ...
   */
}

// Obtaining class instance from document handle
CExportSliceHeadAtXY* CExportSliceHeadAtXY::FromHandle (IfmDocument pDoc)
{
  return reinterpret_cast<CExportSliceHeadAtXY*>(IfmDocumentGetUserData(pDoc));
}

// Callbacks
void CExportSliceHeadAtXY::OnActivate (IfmDocument pDoc, Widget button)
{
  /* 
   * TODO: Add your own code here ...
   */
	if (ExportSliceHeadAtXY(pDoc) != 0)
	{
		IfmInfo(pDoc, "Error occured when runing the ExportSliceHeadAtXY!");
	}

}


void CExportSliceHeadAtXY::PostTimeStep (IfmDocument pDoc)
{
  /* 
   * TODO: Add your own code here ...
   */
}

void CExportSliceHeadAtXY::PostFlowSimulation (IfmDocument pDoc)
{
  /* 
   * TODO: Add your own code here ...
   */
	if (ExportSliceHeadAtXY(pDoc) != 0)
	{
		IfmInfo(pDoc, "Error occured when runing the ExportSliceHeadAtXY!");
	}

}

int CExportSliceHeadAtXY::ExportSliceHeadAtXY(IfmDocument pDoc)
{

	using namespace std;
	double* pt_X;
	double* pt_Y;
	double* pt_Z;
	std::string* pt_ID;
	int*  pt_slice;
	int iTimeStatus; //check the time status of the problem
	
	double* pt_WL;

	double x0, y0, z0;

	int nn, nnSlice, nLay;
	int i;
	int j;

	
	
	//const char* problemPath;
	string  problemPath;
	string problemName;
	stringstream lno;
	ofstream fn;

	nn = IfmGetNumberOfNodes(pDoc);
	nLay = IfmGetNumberOfLayers(pDoc);
	nnSlice = IfmGetNumberOfSlices(pDoc);
	x0 = IfmGetOriginX(pDoc);
	y0 = IfmGetOriginY(pDoc);
	z0 = IfmGetOriginZ(pDoc);
	iTimeStatus = IfmGetTimeClass(pDoc);

	problemPath = IfmGetProblemPath(pDoc);

	problemName = IfmGetProblemTitle(pDoc);

	problemPath.resize(problemPath.length() - problemName.length(), '+');
	_chdir(problemPath.c_str());




	//IfmInfo(pDoc,"export the result head to a file");
	//IfmInfo(m_pDoc,"Export the final head to the file: head.out ....");

	
	//Print in the information windows
	IfmInfo(pDoc, "----------------------------------------------");
	IfmInfo(pDoc, "ExportSliceHeadAtXY started.....");
	IfmInfo(pDoc, "----------------------------------------------");

	nn = IfmGetNumberOfNodes(pDoc);
	//open the output file
	ifstream fin;
	IfmInfo(pDoc, "Open the input file ExportSliceHeadAtXY.inp... ");
	fin.open("ExportSliceHeadAtXY.inp", ios_base::in);

	int nTotalPoint;
	if (fin.is_open())
	{

		//readin the input data
		IfmInfo(m_pDoc, "Read data from the input file ExportSliceHeadAtXY.inp... ");
		fin >> nTotalPoint;

		pt_ID = new string[nTotalPoint];
		pt_X = new double[nTotalPoint];    	//x location of the point
		pt_Y = new double[nTotalPoint];		//y location of the point
		pt_Z = new double[nTotalPoint];		//Z slice elevation at the point
		pt_WL = new double[nTotalPoint];	//point slice water level
		pt_slice = new int[nTotalPoint];      //slice no. 


		for (i = 0; i < nTotalPoint; i++)
		{
			fin >> pt_ID[i] >> pt_X[i] >> pt_Y[i] >> pt_slice[i];
			pt_WL[i] = 0.0;
		}

		fin.close();
	}
	else
	{
		IfmInfo(m_pDoc, "Cann't open the input file, check if the file 'ExportSliceHeadAtXY.inp' in the model directory and the data are in a correct format!!!");
		//delete pt_ID;
		//delete pt_WL;
		//delete pt_X;
		//delete pt_Y;
		//delete pt_Z;
		//delete pt_slice;
		return 1;
	}

	//Retrive the water level at the selected points
	IfmInfo(pDoc, "Open the default output file ExportSliceHeadAtXY.out... ");
	
	//Check the time status of the model
	if (iTimeStatus == 0|| iTimeStatus == 2 )
	{
		//the flow model is steady state
		fn.open("ExportSliceHeadAtXY.out", ios_base::out);
	}
	else if(iTimeStatus == 1 && iTransientFileHeadSwitch==0)
	{
		//the flow model is transient state, but it is the first time step
		fn.open("ExportSliceHeadAtXY.out", ios_base::out);
		
	}
	else if (iTimeStatus == 1 && iTransientFileHeadSwitch == 1)
	{
		//the flow model is transient state, but it is not the first time step, 
		fn.open("ExportSliceHeadAtXY.out", ios_base::app);
	}
	else
	{
		//The time class of the model is not right, it is neither 0 nor 1 or 2. 
		IfmInfo(pDoc, "the time class of the model is wrong, can not run ExportSliceHeadAtXY...");
		return 1;
	}
	
	if(fn.is_open())
	{
		IfmInfo(pDoc, "Write the simualted head to the output file ExportSliceHeadAtXY.out... ");
		fn.setf(ios_base::fixed, ios_base::floatfield);
		if (iTimeStatus == 0 || iTimeStatus == 2)
		{
			fn << "ID" << "\t" << "X" << "\t" << "Y" << "\t" << "Z" << "\t" << "Slice" << "\t" << "WL" << endl;  //print the heading of the file
		}
		else if (iTimeStatus == 1 && iTransientFileHeadSwitch==0) //transient flow model, and first time step and need the heading
		{
			fn << "ID" << "\t" << "X" << "\t" << "Y" << "\t" << "Z" << "\t" << "Slice" << "\t" << "WL" << "\t"<<"Time"<<endl;  //print the heading of the file
			iTransientFileHeadSwitch = 1;
		}
		for (j = 0; j<nTotalPoint; j++)
		{
			IfmBool inDomain(false);
			pt_WL[j] = IfmGetResultsFlowHeadValueAtXYSlice(pDoc, pt_X[j] - x0, pt_Y[j] - y0, pt_slice[j]-1, &inDomain);
			pt_Z[j]=IfmGetZValueAtXYSlice(pDoc, pt_X[j] - x0, pt_Y[j] - y0, pt_slice[j]-1, &inDomain);
			if (inDomain)
			{
				if (iTimeStatus == 0 || iTimeStatus == 2) {
					fn << pt_ID[j] << "\t" << pt_X[j] << "\t" << pt_Y[j] << "\t" << pt_Z[j] + z0 << "\t" << pt_slice[j] << "\t" << pt_WL[j] + z0 << endl;
				}
				else if (iTimeStatus == 1) {
					fn << pt_ID[j] << "\t" << pt_X[j] << "\t" << pt_Y[j] << "\t" << pt_Z[j] + z0 << "\t" << pt_slice[j] << "\t" << pt_WL[j] + z0 << "\t" << IfmGetAbsoluteSimulationTime(pDoc)<< endl;
				}

			}
			else
			{
				fn << pt_ID[j] << "\t" << pt_X[j] << "\t" << pt_Y[j] <<"\t"<<pt_Z[j]+z0<<"\t"<<pt_slice[j]<< "\t" << "outside model domain" << endl;
			}

		}
		
		fn.close();
		delete pt_slice, pt_ID, pt_WL, pt_X, pt_Y, pt_Z;
	}
	else
	{
		IfmInfo(m_pDoc, "Cann't open the out file, error occured!!!");
		delete pt_slice, pt_ID, pt_WL, pt_X, pt_Y, pt_Z;
		return 1;
	}
	IfmInfo(m_pDoc, "Finished running the ExportSliceHeadAtXY plug-in.");
	return 0;
}
