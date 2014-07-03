//
//  JSONDocument.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 25/02/14.
//
//

#include "JSONDocument.h"

string JSONDocument::toString()
{
    if (docId.c_str()!=NULL && jsonDoc.c_str()!=NULL)
    {
        string str = docId;
        str.append(" : ");
        str.append(jsonDoc);
        return str;
    }
    else
    {
        return jsonDoc;
    }
}