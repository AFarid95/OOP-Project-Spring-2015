#include "StepByStep.h"

#include "../ApplicationManager.h"
#include "../Statements/Conditional.h"

#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
StepByStep::StepByStep(ApplicationManager *pAppManager):Action(pAppManager)
{}

void StepByStep::ReadActionParameters()
{}

void StepByStep::Execute()
{
	pIn=pManager->GetInput();
	pOut=pManager->GetOutput();

	if(pManager->ValidateFlowchart())
	{
		Statement* pStat;
		for(int i=0;i<pManager->GetStatCount();i++)
		{
			if(pManager->GetStatList()[i]->GetType()==START)
			{
				pStat=pManager->GetStatList()[i];
				pManager->SetSelectedStatement(pStat);
				pOut->PrintMessage("Step by step run has started. Press Enter to go to the next step. Press Escape to stop");
				pManager->UpdateInterface();
				break;
			}
		}

		char c;
		c=pIn->GetCharPressed();

		while(pStat->GetType()!=END&&c!=1)
		{
			pStat=pStat->Simulate(true);
			pManager->SetSelectedStatement(pStat);
			pManager->UpdateInterface();
			c=0;
			if(pStat==NULL)
				break;
			while(c!=13&&c!=1)
				c=pIn->GetCharPressed();
		}
		if(pStat==NULL)
		{
			if(pIn->GetCancelDoubleValue())
			{
				pOut->PrintMessage("The flowchart run has been stopped");
				pIn->InitializeCancelDoubleValue();
			}
			else
				pOut->PrintMessage("Simulation has been stopped; a variable is being used without being initialized");
		}
		else if(pStat->GetType()==END)
			pManager->SetSelectedStatement(pStat->Simulate(true));
		else if(c==1)
				pOut->PrintMessage("The step by step run has been stopped");
		pManager->SetSelectedStatement(NULL);
	}
}