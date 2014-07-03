/**********************************************************************
 *  QueryBuilder.h
 *  App42CPPSDK
 *
 *  Created by shephertz technologies on 20/02/14.
 *  Copyright (c) 2013 ShephertzTechnology PVT LTD. All rights reserved.
 **********************************************************************/

#ifndef __App42CPPSDK__QueryBuilder__
#define __App42CPPSDK__QueryBuilder__

#include <iostream>
#include "Query.h"
#include <vector>

class QueryBuilder
{
public:
    
    QueryBuilder();
    
    ~QueryBuilder();
    static Query* BuildQuery(std::string key,std::string value,std::string op);
    static Query* BuildQuery(std::string key,std::vector<std::string> value,std::string op);

    static Query* CompoundOperator(Query *q1,std::string op,Query *q2);
    static Query* SetLoggedInUser(std::string logged);

protected:
    
private:
    
};


#endif /* defined(__App42CPPSDK__QueryBuilder__) */
