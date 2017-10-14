#include "MCDM_Algorithms.h"

namespace McdaAlg {};
namespace McdaAlg
{
// parse string into vector of strings
std::vector<std::string> split(const std::string &input, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

Matrix parseInputString(const std::string &input, char delim, int critNumb)
{
    std::vector<std::string> out;
    out=split(input,delim);                 //parse to vector of strings
    int lengthOfString = out.size();
    int numberOfAlternatives = lengthOfString/critNumb;     //define number of alternatives (always int)
    Matrix outNumb(numberOfAlternatives,critNumb);               // init matrix
    for (int j=0; j<numberOfAlternatives; j++)
    {
        for (int i=0; i<critNumb; i++)
        {
            std::istringstream istr(out[i+j*critNumb]);
            std::string temp;
            istr>>temp;
            if (temp=="-inf")   //handle case when there is AP with -inf RSS value
                outNumb.at(j,i) = -100;
            else
                outNumb.at(j,i) = std::stod(temp);
        }
    }
    return outNumb;
}

void showSetNorma(std::vector<Norma> &norm)
{
    int numberOfCrit = norm.size();
    std::cout<<"size of norm structure is : "<<numberOfCrit<<"\n";
    for (int i=0; i<numberOfCrit; i++)
    {
        std::cout<<"The name of the criteria is \t"<<norm[i].nameCrit<<"\n";
        std::cout<<"Is the type of the criteria upward? \t"<<norm[i].upwardType<<"\n";
        std::cout<<"The upper bound is \t"<<norm[i].u<<"\n";
        std::cout<<"The lower bound is \t"<<norm[i].l<<"\n"<<"-------\n";
    }
}

//read parameters fro enhMaxMin from file enhNorm.dat
std::vector<Norma> setNorma(int critNumb)
{
    std::vector<Norma> norm;
    std::ifstream inf("/home/radion/omnetpp-4.6/inetMCDM/src/inet/linklayer/ieee80211/mgmt/mcda/enhNorm"+std::to_string(critNumb)+".dat");
    // If we couldn't open the output file stream for reading
    if (!inf)
    {
        // Print an error and exit
        std::cerr << "Uh oh, file with normalization values could not be opened for reading!" <<"\n";
        exit(1);
    }
    // While there's still stuff left to read
    while (inf)
    {
        // read stuff from the file into matrix A elements
        for (int i=0; i<critNumb; i++)
        {
            //Push back new subject created with default constructor.
            norm.push_back(Norma());
            //Vector now has 1 element @ index 0, so modify it.
            inf>>norm[i].nameCrit;
            inf>>norm[i].upwardType;
            inf>>norm[i].u;
            inf>>norm[i].l;
        }
    }
    norm.resize(critNumb); //assure proper size
    return norm;
}

std::vector<Norma> selectNormCriteria(std::vector<Norma> norm, Matrix decisionCriteriaIndexes)
{
    std::vector<Norma> normCut;
    int numbOfCutCriteria=decisionCriteriaIndexes.size(1);
    for (int i=0; i<numbOfCutCriteria; i++)
    {
        //Push back new subject created with default constructor.
        normCut.push_back(Norma());
        //Vector now has 1 element @ index 0, so modify it.
        normCut[i].nameCrit = norm[decisionCriteriaIndexes.at(i,0)].nameCrit;
        normCut[i].upwardType = norm[decisionCriteriaIndexes.at(i,0)].upwardType;
        normCut[i].u = norm[decisionCriteriaIndexes.at(i,0)].u;
        normCut[i].l = norm[decisionCriteriaIndexes.at(i,0)].l;
    }
    normCut.resize(numbOfCutCriteria); //assure proper size
    return normCut;
}


Matrix enhMaxMin(Matrix a, std::vector<Norma> norm)
{
    int m = a.size(1);
    int n = a.size(2);
    Matrix d(m,n);
    for (int j=0; j<n; j++)
    {
        if (norm[j].upwardType == true)
        {
            for (int i=0; i<m; i++)
            {
                d.at(i,j) = (a.at(i,j) - norm[j].l) / (norm[j].u-norm[j].l);
                //std::cout<<d.at(i,j)<<"\n";
            }
        }
        else
        {
            for (int i=0; i<m; i++)
            {
                d.at(i,j) = (norm[j].u - a.at(i,j)) / (norm[j].u-norm[j].l);
                //std::cout<<d.at(i,j)<<"\n";
            }
        }
    }
    return d;
}

Matrix readPreferences(std::string trafficType, int critNumb)
{

    Matrix A(critNumb,critNumb);
    std::ifstream inf("/home/radion/omnetpp-4.6/inetMCDM/src/inet/linklayer/ieee80211/mgmt/mcda/A"+std::to_string(critNumb)+trafficType+".dat");
    // If we couldn't open the output file stream for reading
    if (!inf)
    {
        // Print an error and exit
        std::cerr << "Uh oh, file with preferences could not be opened for reading!" <<"\n";
        exit(1);
    }
    // While there's still stuff left to read
    while (inf)
    {
        // read stuff from the file into matrix A elements
        for (int i=0; i<critNumb; i++)
        {
            for (int j=0; j<critNumb; j++)
            {
                inf>>A.at(i,j);
                //std::cout<<A.at(i,j)<<"\n";
            }
        }
    }
    return A;
}

Matrix wls_weighting(Matrix A)
{
    int critNumb = A.size(1);
    Matrix e(critNumb,1);
    e = ones(critNumb,1);
    Matrix B(critNumb,critNumb);
    B = diag(transpose(A)*A) - A - transpose(A) + critNumb*eye(critNumb);
    Matrix w(critNumb,1);
    w = inv(B)* e / (transpose(e)*inv(B)*e);
    return w;
}

//find ideal solution for MCDA
Matrix idealSolution(Matrix D)
{
    int critNumb=D.size(2);
    Matrix ideal(1,critNumb);
    Matrix idealIndex(1,critNumb);
    for (int i=0; i<critNumb; i++)
    {
        ideal.at(0,i) = D.at(maxIndex(D,i,"column"),i);
    }
    return ideal;
}

//find anti-ideal solution for MCDA
Matrix antiIdealSolution(Matrix D)
{
    int critNumb=D.size(2);
    Matrix antiIdeal(1,critNumb);
    Matrix antiIdealIndex(1,critNumb);
    for (int i=0; i<critNumb; i++)
    {
        antiIdeal.at(0,i) = D.at(minIndex(D,i,"column"),i);
    }
    return antiIdeal;
}

Matrix SAW(Matrix D, Matrix W)
{
    Matrix score(D.size(1),W.size(2));
    score = D * W;
    return score;
}

Matrix GRA(Matrix D, Matrix W)
{
    int altNumb=D.size(1);
    int critNumb=D.size(2);
    Matrix score(D.size(1),1);
    Matrix ideal = idealSolution(D);
    for (int i=0; i<altNumb; i++)
    {
        double stuff {0};
        for (int j=0; j<critNumb; j++)
        {
            stuff += W.at(j,0) * fabs(D.at(i,j)-ideal.at(0,j));
        }
        score.at(i,0) = 1 / (stuff + 1);
    }
    return score;
}

Matrix TOPSIS(Matrix D, Matrix W)
{
    int altNumb=D.size(1);
    int critNumb=D.size(2);
    Matrix V(altNumb,critNumb);
    for (int i=0; i<altNumb; i++)
    {
        for (int j=0; j<critNumb; j++)
        {
            V.at(i,j) = D.at(i,j) * W.at(j,0);
        }
    }
    Matrix Vplus = idealSolution(V);        //find ideal solution
    Matrix Vminus = antiIdealSolution(V);    // anti-ideal
    Matrix Splus(altNumb,1);
    Matrix Sminus(altNumb,1);
    Matrix RC(altNumb,1);
    for (int i=0; i<altNumb; i++)
    {
        for (int j=0; j<critNumb; j++)
        {
            Splus.at(i,0) += pow((Vplus.at(0,j) - V.at(i,j)),2);
            Sminus.at(i,0) += pow((V.at(i,j) - Vminus.at(0,j) ),2);
        }
        Splus.at(i,0) = pow(Splus.at(i,0), 0.5);
        Sminus.at(i,0) = pow(Sminus.at(i,0), 0.5);
        RC.at(i,0) = Sminus.at(i,0) / (Splus.at(i,0) + Sminus.at(i,0));
    }
    return RC;
}

/*if normalized matrix has values =1 or =0, it is very likely, that
they are the same for several alternatives (from observations).
The idea is to add or substract some very small random value,
such that ideal and antiIdeal vectors will be different.
*/
void checkAndModifyInputForVikor(Matrix &D)
{
    int altNumb=D.size(1);
    int critNumb=D.size(2);
    for (int i=0; i<altNumb; i++)
    {
        for (int j=0; j<critNumb; j++)
        {
            if (D.at(i,j)==0)
            {
                D.at(i,j) = D.at(i,j) + random(0,90)/10000;
            }
            else if(D.at(i,j)==1)
            {
                D.at(i,j) = D.at(i,j) - random(0,90)/10000;
            }
        }
    }
}

// random numbers generated in a given range
double random(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);  // static used for efficiency, so we only calculate this value once
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

//VIKOR version w/o stability checking
Matrix VIKOR(Matrix D, Matrix W)
{
    int altNumb=D.size(1);
    int critNumb=D.size(2);
    checkAndModifyInputForVikor(D);         //in case of identical values
    Matrix ideal = idealSolution(D);        //find ideal solution
    Matrix antiIdeal = antiIdealSolution(D);    // anti-ideal
    //ideal.print();
    //antiIdeal.print();
    Matrix S(altNumb,1);
    Matrix R(altNumb,1);
    for (int i=0; i<altNumb; i++)
    {
        Matrix stuff(1,critNumb);
        for (int j=0; j<critNumb; j++)
        {
            stuff.at(0,j) = W.at(j,0) * (ideal.at(0,j) - D.at(i,j)) / (ideal.at(0,j) - antiIdeal.at(0,j));
        }
        R.at(i,0) = maxElement(stuff,0,"row");
        //R.print();
        S.at(i,0) = sum(stuff,0,"row");
        //S.print();
    }

    double Rplus =  maxElement(R,0,"column");
    //std::cout<<"R+ "<<Rplus<<"\n";
    double Rminus =  minElement(R,0,"column");
    //std::cout<<"R- "<<Rminus<<"\n";
    double Splus =  maxElement(S,0,"column");
    //std::cout<<"S+ "<<Splus<<"\n";
    double Sminus =  minElement(S,0,"column");
    //std::cout<<"S- "<<Sminus<<"\n";

    double v {0.5};     // 0.5 corresponds to consensus strategy
    Matrix Q(altNumb,1);
    for (int i=0; i<altNumb; i++)
    {
        Q.at(i,0)=v*(S.at(i,0)-Sminus)/(Splus-Sminus) +
                  (1-v)*(R.at(i,0)-Rminus)/(Rplus-Rminus);
        //std::cout<<(1-v)*(R.at(i,0)-Rminus)/(Rplus-Rminus) <<"\n";
    }
    //Q.print();
    return Q;
}
//overloaded VIKOR w stability checking
Matrix VIKOR( Matrix D, Matrix W, bool &checkStability)
{
    int altNumb=D.size(1);
    int critNumb=D.size(2);
    checkAndModifyInputForVikor(D);         //in case of identical values
    Matrix ideal = idealSolution(D);        //find ideal solution
    Matrix antiIdeal = antiIdealSolution(D);    // anti-ideal
    Matrix S(altNumb,1);
    Matrix R(altNumb,1);
    for (int i=0; i<altNumb; i++)
    {
        Matrix stuff(1,critNumb);
        for (int j=0; j<critNumb; j++)
        {
            stuff.at(0,j) = W.at(j,0) * (ideal.at(0,j) - D.at(i,j)) / (ideal.at(0,j) - antiIdeal.at(0,j));
        }
        R.at(i,0) = maxElement(stuff,0,"row");
        S.at(i,0) = sum(stuff,0,"row");
    }

    double Rplus =  maxElement(R,0,"column");
    double Rminus =  minElement(R,0,"column");

    double Splus =  maxElement(S,0,"column");
    double Sminus =  minElement(S,0,"column");

    double v {0.4};     // corresponds to consensus strategy
    Matrix Q(altNumb,1);
    for (int i=0; i<altNumb; i++)
    {
        Q.at(i,0)=v*(S.at(i,0)-Sminus)/(Splus-Sminus) +
                  (1-v)*(R.at(i,0)-Rminus)/(Rplus-Rminus);
        //std::cout<<(1-v)*(R.at(i,0)-Rminus)/(Rplus-Rminus) <<"\n";
    }
    //check conditions of acceptable advantage and stability
    if ((maxIndex(Q,0,"column") == maxIndex(S,0,"column") &&
            maxIndex(Q,0,"column") == maxIndex(R,0,"column")) &&
            (  (maxIndex(Q,0,"column")-secondMaxIndex(Q,0,"column")) >= (1/(altNumb-1)) )    )
    {
        checkStability = true;
        std::cout<<"\n Decision is stable \n";
        return Q;
    }
    else
    {
        checkStability = false;
        std::cout<<"\n Decision is NOT stable \n";
        return Q;
    }
}

// here goes decisions by MCDA
void chooseMCDA(Matrix D, Matrix W, std::string algName)
{
    Matrix score(D.size(1),1);
    if (algName == "SAW")
        score = SAW(D,W);
    else if (algName == "GRA")
        score = GRA(D,W);
    else if (algName == "TOPSIS")
        score = TOPSIS(D,W);
    else if (algName == "VIKOR")
    {
//        std::cout<<"Check stability for VIKOR? y/n\t";
//                 char st;           //remove that
//        do
//        {
//            std::cin>>st;
//        }
//        while ((st!='y') && (st!='n'));
//            if (st=='y')
//            {
//                bool check;
//                score = VIKOR(D,W,check);
//            }
//            else
        score = VIKOR(D,W);
    }
    else
        std::cerr<<"Wrong entered name!!\n";

    std::cout<<"Score by "<<algName<<" is: \n";
    score.print();
    int bestIndex=maxIndex(score,0,"column");
    std::cout<<algName<<" selects best index "<<bestIndex<<"\n"<<
             "---------------------------\n\n";
}


Matrix selectSomeCriteria(Matrix A, Matrix decisionCriteriaIndexes)
{
    int cutCritNumb=decisionCriteriaIndexes.size(1);
    int altNumb = A.size(2);
    Matrix Acut(altNumb,cutCritNumb);
    for(int i=0; i<altNumb; ++i)
    {
        for(int j=0; j<cutCritNumb; ++j)
        {
            Acut.at(i,j)=A.at(i,decisionCriteriaIndexes.at(j,0));
        }
    }
    return Acut;
}

int decisionProcess(Matrix goodAlt,std::vector<Norma> norm,int critNumb,
                        std::string trafficType,std::string algName)
{
    //goodAlt.print();
    Matrix D = enhMaxMin(goodAlt,norm);
    //D.print();
    //Matrix D =enhMaxMin(C,norm);
    // weighting stage
    Matrix A = readPreferences(trafficType,critNumb);
    Matrix W = wls_weighting(A);
    //W.print();
    //Matrix W =
    // decision stage
    Matrix score(D.size(1),1);
    if (algName == "SAW")
        score = SAW(D,W);
    else if (algName == "GRA")
        score = GRA(D,W);
    else if (algName == "TOPSIS")
        score = TOPSIS(D,W);
    else if (algName == "VIKOR")
        score = VIKOR(D,W);
    else
        std::cerr<<"Wrong entered name!!\n";
    //score.print();
    int bestIndexFromGood=0;
    if (algName=="VIKOR")
        bestIndexFromGood = minIndex(score,0,"column");
    else
        bestIndexFromGood = maxIndex(score,0,"column");
    return bestIndexFromGood;
}

Matrix selectGoodAlt(Matrix C, double threshold, int numbOfGoodAlt)
{
    int i,j=0;
    Matrix goodAlt(numbOfGoodAlt,C.size(2));
    for(i=0; i<C.size(1); i++)
    {
        //std::cout<<C.at(i,0);
        if(C.at(i,0)>threshold)
        {
            for(int m=0; m<C.size(2); ++m)
            {
                goodAlt.at(j,m)=C.at(i,m);
            }
            ++j;
        }
    }
    return goodAlt;
}


// omnet++ ready function
int selectAPWithMCDA(std::string allPathsCriteriaValues,std::string algName,
                     std::string trafficType, int critNumb)
{
    // normalization stage
    std::vector<Norma> norm = setNorma(critNumb); //set norm parameters
    Matrix C = parseInputString(allPathsCriteriaValues,',',critNumb);
    std::cout<<"\n";
    C.print();
    int altNumb=C.size(1);
    Matrix RSSvalues(altNumb,1);
    Matrix goodAltIndex(altNumb,1);
    int numbOfGoodAlt=0;
    double threshold=-83;
    int bestIndex;
    for(int i=0; i<altNumb; i++)
    {
            if(C.at(i,0)>threshold)
            {
                ++numbOfGoodAlt;
                goodAltIndex.at(i,0)=1;
            }
            RSSvalues.at(i,0)=C.at(i,0);
            //std::cout<<RSSvalues.at(i,0)<<"\n";
    }
    std::cout<<"good alt   "<<numbOfGoodAlt<<"\n";
    if(numbOfGoodAlt==0)
    {
        std::cout<<"There is no good.... \n";
        return bestIndex=maxIndex(RSSvalues,0,"column");
    }
    else
    {
    Matrix goodAlt(numbOfGoodAlt, critNumb);
    goodAlt = selectGoodAlt(C, threshold, numbOfGoodAlt);
    goodAlt.print();
    goodAltIndex.print();
    Matrix D = enhMaxMin(goodAlt,norm);
    D.print();
    // weighting stage
    Matrix A = readPreferences(trafficType,critNumb);
    Matrix W = wls_weighting(A);
    W.print();
    // decision stage
    Matrix score(D.size(1),1);
    if (algName == "SAW")
         score = SAW(D,W);
    else if (algName == "GRA")
        score = GRA(D,W);
    else if (algName == "TOPSIS")
        score = TOPSIS(D,W);
    else if (algName == "VIKOR")
        score = VIKOR(D,W);
    else
        std::cerr<<"Wrong entered name!!\n";
    //score.print();
    int bestIndexFromGood=0;
    if (algName=="VIKOR")
        bestIndexFromGood = minIndex(score,0,"column");
    else
        bestIndexFromGood = maxIndex(score,0,"column");
    //int bestIndexFromGood=maxIndex(score,0,"column");
    std::cout<<"best Index fr G  "<<bestIndexFromGood<<"\n";
    int bestDummy=0;
    for(int i=0; i<altNumb; ++i)
    {
        //++bestIndex;
        if(goodAltIndex.at(i,0)==1)
        {
            if(bestDummy==bestIndexFromGood)
            {
                return bestIndex=i;
            }
            ++bestDummy;
        }
    }
    }
    return bestIndex;
}

}
