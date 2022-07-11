#include "StdAfx.h"
#include "ExportBasement.h"

ExportBasement::ExportBasement(Saver& saver_,const char* name_)
:saver(saver_)
{
	name=name_;
}

void ExportBasement::Export(IGameMesh* pobject_,IGameNode* node,Matrix3 root_node_matrix)
{
	pobject=pobject_;

	Matrix3 m_node=node->GetWorldTM(pRootExport->GetBaseFrameMax()).ExtractMatrix3();
	Matrix3 inv_node=Inverse(m_node);
	Matrix3 inv_root_matrix=Inverse(root_node_matrix);
	pobject->InitializeData();
	if(pobject->GetNumberOfVerts()<=0)
	{
		Msg("Error: %s cannot have vertex.\n",name);
		return;
	}

	if(pobject->GetNumberOfFaces()<=0)
	{
		Msg("Error: %s cannot have faces.\n",name);
		return;
	}

	saver.push(C3DX_BASEMENT);

	{
		saver.push(C3DX_BASEMENT_FACES);
		int num_faces=pobject->GetNumberOfFaces();
		saver<<num_faces;
		for(int i=0;i<num_faces;i++)
		{
			WORD pp;
			FaceEx* face=pobject->GetFace(i);
			pp=face->vert[0];
			saver<<pp;
			pp=face->vert[1];
			saver<<pp;
			pp=face->vert[2];
			saver<<pp;
		}
		saver.pop();
	}

	{
		saver.push(C3DX_BASEMENT_VERTEX);
		int num_vertex=pobject->GetNumberOfVerts();
		saver<<num_vertex;
		for(int i=0;i<num_vertex;i++)
		{
			Point3 pos0=pobject->GetVertex(i);
			Point3 pos=pos0*inv_root_matrix;
			Vect3f p(pos.x,pos.y,pos.z);
			RightToLeft(p);
			saver<<p;
		}
		saver.pop();
	}

	saver.pop();
}
