//
//  App42Score.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 25/02/14.
//
//

#ifndef __App42CPPSDK__App42Score__
#define __App42CPPSDK__App42Score__

#include <iostream>
#include "MetaResponse.h"

class App42Score : public MetaResponse
{
    public:
    
    /**
     * Getters
     */
    string getUserName(){return userName;};
    string getRank(){return rank;};
    string getCreatedOn(){return createdOn;};
    string getScoreId(){return scoreId;};
    double getScoreValue(){return value;};

    /**
     * Setters
     */
    void setUserName(string _userName){userName=_userName;};
    void setRank(string _rank){ rank=_rank;};
    void setCreatedOn(string _createdOn){ createdOn=_createdOn;};
    void setScoreId(string _scoreId){ scoreId=_scoreId;};
    void setScoreValue(double _value){ value=_value;};

    protected:
    
    std::string userName;
    std::string rank;
    std::string createdOn;
    std::string scoreId;
    double value;

    private:
};

#endif /* defined(__App42CPPSDK__App42Score__) */
