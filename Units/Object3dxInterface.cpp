#include "stdafx.h"
#include "Object3dxInterface.h"
#include "UnitAttribute.h"
#include "Console.h"
#include "Render\src\Scene.h"
#include "Render\src\VisGeneric.h"

int VisibilityGroupOfSet::visibilitySet_;

StringIndexBase::StringIndexBase()
: index_(0)
{}

void StringIndexBase::set(const char* name)
{
	name_ = name;
}

bool StringIndexBase::serialize(Archive& ar, const char* name, const char* nameAlt)
{
	if(ar.isOutput())
		update();
	if(ar.isEdit()){
		return ar.serialize(name_, name, nameAlt);
	}
	else if(ar.openStruct(*this, name, nameAlt)){
		ar.serialize(name_, "name", 0);
		ar.serialize(index_, "index", 0);
		ar.closeStruct(name);
		return true;
	}
	return false;
}

void ChainName::update()
{
	cObject3dx* model = AttributeBase::model();
	if(!model)
		return;

	index_ = model->GetChainIndex(name_);
	if(index_ == -1){
		if(strcmp(name_, "main") != 0)
			kdWarning("&units", XBuffer(1024, 1) < /*TRANSLATE*/("������������ ������� �� ������� : ") < model->GetFileName() < " : " < name_);

		name_ = model->GetChain(index_ = 0)->name.c_str();
	}

	string comboList;
	int number = model->GetChainNumber();
	for(int i = 0; i < number; i++){
		if(!comboList.empty())
			comboList += "|";
		comboList += model->GetChain(i)->name;
	}
	name_.setComboList(comboList.c_str());
	if(number && !strlen(name_))
		name_ = model->GetChain(0)->name;
}

void AnimationGroupName::update()
{
	cObject3dx* model = AttributeBase::model();
	if(!model)
		return;


	if(name_.value() == "All animation group")
		index_ = -1;
	else {
		index_ = model->GetAnimationGroup(name_);
		if(index_ == -1){
			index_ = 0;
			if(model->GetAnimationGroupNumber())
				name_ = model->GetAnimationGroupName(0);
		}
	}

	string comboList;
	int number = model->GetAnimationGroupNumber();
	for(int i = 0; i < number; i++){
		if(!comboList.empty())
			comboList += "|";
		comboList += model->GetAnimationGroupName(i);
	}
	comboList += "|All animation group";
	name_.setComboList(comboList.c_str());
	if(number && !strlen(name_))
		name_ = model->GetAnimationGroupName(0);
}

void VisibilityGroupName::update()
{
	cObject3dx* model = AttributeBase::model();
	if(!model)
		return;

	index_ = model->GetVisibilityGroupIndex(name_);
	if(index_ == -1)
	{
		index_=0;
		name_ = model->GetVisibilityGroupName(VisibilityGroupIndex(index_));
	}

	string comboList;
	int number = model->GetVisibilityGroupNumber();
	for(int i = 0; i < number; i++){
		if(!comboList.empty())
			comboList += "|";
		comboList += model->GetVisibilityGroupName(VisibilityGroupIndex(i));
	}
	name_.setComboList(comboList.c_str());
	if(number && !strlen(name_))
		name_ = model->GetVisibilityGroupName(VisibilityGroupIndex(0));
}

void VisibilitySetName::update()
{
	cObject3dx* model = AttributeBase::model();
	if(!model)
		return;

	index_ = model->GetVisibilitySetIndex(name_);
	if(index_ == -1)
		name_ = model->GetVisibilitySetName(VisibilitySetIndex(index_ = 0));

	string comboList;
	int number = model->GetVisibilitySetNumber();
	for(int i = 0; i < number; i++){
		if(!comboList.empty())
			comboList += "|";
		comboList += model->GetVisibilitySetName(VisibilitySetIndex(i));
	}
	name_.setComboList(comboList.c_str());
	if(number && !strlen(name_))
		name_ = model->GetVisibilitySetName(VisibilitySetIndex(0));
}

void VisibilityGroupOfSet::update()
{
	cObject3dx* model = AttributeBase::model();
	if(!model)
		return;

	index_ = model->GetVisibilityGroupIndex(name_, VisibilitySetIndex(visibilitySet_));
	if(index_ == -1)
		name_ = model->GetVisibilityGroupName(VisibilityGroupIndex(index_ = 0), VisibilitySetIndex(visibilitySet_));

	string comboList;
	int number = model->GetVisibilityGroupNumber(VisibilitySetIndex(visibilitySet_));
	for(int i = 0; i < number; i++){
		if(!comboList.empty())
			comboList += "|";
		comboList += model->GetVisibilityGroupName(VisibilityGroupIndex(i), VisibilitySetIndex(visibilitySet_));
	}
	name_.setComboList(comboList.c_str());
	if(number && !strlen(name_))
		name_ = model->GetVisibilityGroupName(VisibilityGroupIndex(0), VisibilitySetIndex(visibilitySet_));
}

void GenericObject3dxNode::update()
{
	if(!modelName_ || !*modelName_)
		return;
	if(cScene* scene = gb_VisGeneric->CreateScene()){
		if(cObject3dx* model = scene->CreateObject3dx(modelName_)){
			string comboList;
			int number = model->GetNodeNumber();
			for(int idx = 0; idx < number; ++idx){
				comboList += "|";
				comboList += model->GetNodeName(idx);
			}
			name_.setComboList(comboList.c_str());
			index_ = model->FindNode(name_);
			if(index_ == -1){
				index_ = 0;
				name_ = model->GetNodeName(index_);
			}

			model->Release();
		}
		scene->Release();
	}
}

void Logic3dxNode::updateInternal(bool logic)
{
	if(cObject3dx* model = logic ? AttributeBase::logicModel() : AttributeBase::model()){
		if(!model)
			return;
		string comboList = "";
		int number = model->GetNodeNumber();
		for(int i = 0; i < number; i++){
			comboList += "|";
			comboList += model->GetNodeName(i);
		}
		name_.setComboList(comboList.c_str());
		index_ = model->FindNode(name_);
		if(index_ == -1){
			index_ = 0;
			name_ = model->GetNodeName(index_);
		}
	}
}

void Logic3dxNodeBound::updateInternal(bool logic)
{
	if(cObject3dx* model = logic ? AttributeBase::logicModel() : AttributeBase::model()){
		bool inited(true);
		index_ = model->FindNode(name_);
		if(index_ == -1){
			index_ = 0;
			inited = false;
		}
		string comboList = "";
		int number = model->GetNodeNumber();
		for(int i = 0; i < number; i++){
			if(model->IsLogicBound(i)){
				comboList += "|";
				if(inited){
					comboList += model->GetNodeName(i);	
				} else {
					name_ = model->GetNodeName(i);
					comboList += name_;
					inited = true;
				}
			}
		}
		name_.setComboList(comboList.c_str());
	}
}
