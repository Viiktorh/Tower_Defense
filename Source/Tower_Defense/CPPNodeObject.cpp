// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPNodeObject.h"


UCPPNodeObject* UCPPNodeObject::Node(FString Data)
{
	mData = Data;
	return this;
}
