//
//  JSONDocument.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 25/02/14.
//
//

#ifndef __App42CPPSDK__JSONDocument__
#define __App42CPPSDK__JSONDocument__

#include <iostream>

using namespace std;

class JSONDocument
{
public:
/**
 * Getters
 */
    string getJsonDoc(){return jsonDoc;};
    string getDocId(){return docId;};
    string getEvent(){return event;};
    string getCreatedAt()
    {
        return createdAt;
    };
    string getUpdatedAt(){return updatedAt;};
    string getOwner(){return owner;};
    
/**
 * Setters
 */
    void setJsonDoc(string _jsonDoc){jsonDoc=_jsonDoc;};
    void setDocId(string _docId){docId=_docId;};
    void setEvent(string _event){event=_event;};
    void setCreatedAt(string _createdAt){createdAt=_createdAt;};
    void setUpdatedAt(string _updatedAt){updatedAt=_updatedAt;};
    void setOwner(string _owner){owner=_owner;};


    string toString();
    
protected:
    string jsonDoc;
    string createdAt;
    string event;
    string updatedAt;
    string docId;
    
private:
    string owner;
};
#endif /* defined(__App42CPPSDK__JSONDocument__) */
