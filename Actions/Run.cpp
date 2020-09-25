#include "Run.h"

#include "../ApplicationManager.h"
#include "../Statements/Conditional.h"

#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
Run::Run(ApplicationManager *pAppManager):Action(pAppManager)
{}

void Run::ReadActionParameters()
{}

void Run::Execute()
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
				break;
			}
		}
		while(pStat->GetType()!=END)
		{
			pStat=pStat->Simulate();
			if(pStat==NULL)
				break;
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
		else
			pStat=pStat->Simulate();
	}
}