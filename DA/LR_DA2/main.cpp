#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>

const char BLACK = 1;
const char RED = 2;

class TRBNode {
public: 
    char* Key; 
    unsigned long long Value;
    char Colour = BLACK;

    TRBNode* Parent = nullptr;
    TRBNode* Left = nullptr;
    TRBNode* Right = nullptr; 

    TRBNode() {
        Key = (char*)calloc(260,sizeof(char));
        if(Key == nullptr) {
            throw std::bad_alloc();
        }
        Value = 0;
        Key = strcpy(Key,"");
    }

    TRBNode(char* inpkey,unsigned long long inpval) {
        Key = (char*)calloc(260,sizeof(char));
        if(Key == nullptr) {
            throw std::bad_alloc();
        }
        Key = strcpy(Key,inpkey); 
        Value = inpval;
    }

    TRBNode(unsigned long long inpval, char* inpkey) {
        Key = (char*)calloc(260,sizeof(char));
        if(Key == nullptr) {
            throw std::bad_alloc();
        }
        Key = strcpy(Key,inpkey); 
        Value = inpval;
    }

    ~TRBNode() {
        free(Key);
    }

    TRBNode* Sibling() {
        if (Parent != nullptr && Parent->Left != nullptr && Parent->Right != nullptr) {
            if (Parent->Left->Key == Key) {
                return Parent->Right; 
            }
            else {
                return Parent->Left;
            }
        }
        else {
            return nullptr;
        }
    }

    bool Isleft() {
        if (Parent != nullptr && Parent->Left != nullptr && Parent->Left->Key != nullptr && Parent->Left->Key == Key) {
            return true; 
        }
        else {
            return false;
        }
    }

    bool Islist() {
        if (Right == nullptr || Left == nullptr) {
            return true; 
        }
        else {
            return false;
        }
    }
};

int strequal (const char* lhs, const char* rhs) {
    if(lhs == nullptr || rhs == nullptr) {
        if(lhs == nullptr) {
            return -1;
        }
        if(rhs == nullptr) {
            return 1;
        }
    }

    size_t i=0;
    while (lhs[i]!=0 || rhs[i]!=0) {
        if (lhs[i] == rhs[i]) {
            ++i;
            continue;
        }
        else {
            char cmp_l = lhs[i];
            char cmp_r = rhs[i];
            if(lhs[i] >= 'A' && lhs[i] <= 'Z') {
                cmp_l = lhs[i] + ('a' - 'A');
            } 
            if(rhs[i] >= 'A' && rhs[i] <= 'Z') {
                cmp_r = rhs[i] + ('a' - 'A');
            } 
            
            if(cmp_l < cmp_r) {
                return -1;
            } else 
            if(cmp_l > cmp_r) {
                return 1;
            } else {
                ++i;
                continue;
            }
        }
    }
    if(lhs[i]!=0 || rhs[i]!=0)
    {
        if(lhs[i]==0) {
            return -1;
        } 
        if(rhs[i]==0) {
            return 1;
        }
    }

    return 0;
}

void Tree_print(TRBNode* start, int counter) { 
    if (start == nullptr) {
        return;
    }

    Tree_print(start->Right, counter + 1);
    for (int i = 0; i < counter; ++i) {
        printf("\t");
    }
    printf("( ");
    if(start->Parent!=nullptr) {
        printf("%s<-p|",start->Parent->Key);
    }
    else {
        printf("nullptr <-p|");
    }
    printf("%s",start->Key);
    printf(" | %llu ) - | ", start->Value);
    if (start->Colour == RED) {
        printf("RED");
    }
    else {
        printf("BLACK");
    }
    printf("|\n");
    Tree_print(start->Left, counter + 1);
}

void Swap(TRBNode* lhs, TRBNode* rhs) {
    char* tmpkey = (char*)calloc(300,sizeof(char));

    if(tmpkey == nullptr) {
        throw -1;
    }

    tmpkey = strcpy(tmpkey,rhs->Key);
    unsigned long long tmpval = rhs->Value;

    rhs->Key = strcpy(rhs->Key,lhs->Key);
    rhs->Value = lhs->Value; 

    lhs->Key = strcpy(lhs->Key,tmpkey);
    lhs->Value = tmpval;
    free(tmpkey);
}

TRBNode* LeftTurn(TRBNode* centre) {
    if (centre->Parent != nullptr) {
        if (centre->Parent->Left == centre) {
            centre->Parent->Left = centre->Left;
        }
        else
        if (centre->Parent->Right == centre) {
            centre->Parent->Right = centre->Left;
        }
    }

    TRBNode* new_centre = centre->Left;
    centre->Left = centre->Left->Right;
    centre->Left->Parent = centre;
    
    new_centre->Parent = centre->Parent;
    new_centre->Right = centre;
    centre->Parent = new_centre;


    return new_centre;
}

TRBNode* RightTurn(TRBNode* centre) {
    if (centre->Parent != nullptr) {
        if (centre->Parent->Left == centre) {
            centre->Parent->Left = centre->Right;
        }
        else
        if (centre->Parent->Right == centre) {
            centre->Parent->Right = centre->Right;
        }
    }

    TRBNode* new_centre = centre->Right;
    centre->Right = centre->Right->Left;
    centre->Right->Parent = centre;

    new_centre->Parent = centre->Parent;
    new_centre->Left = centre;
    centre->Parent = new_centre;


    return new_centre;
}



TRBNode* ARMreballance(TRBNode* start_element, TRBNode* root) {
    if (root == start_element) {
        return root;
    }
    else {
        TRBNode* sibl = start_element->Sibling();
    
        if (sibl->Colour == RED) {
            if (sibl->Isleft()) {
                
                if (start_element->Parent == root) {
                    root = LeftTurn(start_element->Parent);
                }
                else {
                    LeftTurn(start_element->Parent);
                }

                sibl->Colour = BLACK;
                sibl->Right->Colour = RED;//

                root = ARMreballance(start_element, root);
            }
            else {
                if (start_element->Parent == root) {
                    root = RightTurn(start_element->Parent);
                }
                else {
                    RightTurn(start_element->Parent);
                }

                sibl->Colour = BLACK;
                sibl->Left->Colour = RED;

                root = ARMreballance(start_element, root);
            }
        }
        else {
            if (sibl->Isleft()) {
                if (sibl->Left->Colour == BLACK && sibl->Right->Colour == BLACK && sibl->Parent->Colour == BLACK) {
                    sibl->Colour = RED;
                    sibl->Parent->Colour = BLACK;

                    root = ARMreballance(start_element->Parent, root);
                }
                else
                if (sibl->Left->Colour == BLACK && sibl->Right->Colour == BLACK && sibl->Parent->Colour == RED) {
                    sibl->Colour = RED;
                    sibl->Parent->Colour = BLACK;
                }
                else
                if (sibl->Right->Colour == RED && sibl->Left->Colour == BLACK) {
                    sibl->Right->Colour = BLACK;
                    sibl->Colour = RED;
                    RightTurn(sibl);
                    
                    root = ARMreballance(start_element, root);
                }
                else
                if (sibl->Left->Colour == RED) {
                    sibl->Colour = sibl->Parent->Colour;
                    sibl->Parent->Colour = BLACK;
                    //sibl->Right->Colour = BLACK;
                    sibl->Left->Colour = BLACK;

                    if (sibl->Parent == root) {
                        root = LeftTurn(sibl->Parent);
                    }
                    else {
                        LeftTurn(sibl->Parent);
                    }
                }
            }
            else
            {
                if (sibl->Left->Colour == BLACK && sibl->Right->Colour == BLACK && sibl->Parent->Colour == BLACK) {
                    sibl->Colour = RED;
                    sibl->Parent->Colour = BLACK;

                    root = ARMreballance(start_element->Parent, root);
                }
                else
                if (sibl->Left->Colour == BLACK && sibl->Right->Colour == BLACK && sibl->Parent->Colour == RED) {
                    sibl->Colour = RED;
                    sibl->Parent->Colour = BLACK;
                }
                else
                if (sibl->Left->Colour == RED && sibl->Right->Colour == BLACK) {
                    sibl->Left->Colour = BLACK;
                    sibl->Colour = RED;
                    LeftTurn(sibl);

                    root = ARMreballance(start_element, root);
                }
                else
                if (sibl->Right->Colour == RED) {
                    sibl->Colour = sibl->Parent->Colour;
                    sibl->Parent->Colour = BLACK;
                    sibl->Right->Colour = BLACK;

                    if (sibl->Parent == root) {
                        root = RightTurn(sibl->Parent);
                    }
                    else {
                        RightTurn(sibl->Parent);
                    }
                }
            }
        }

        return root;
    }
}

TRBNode* AISreballance(TRBNode* start_element,TRBNode* root) {

    if (start_element->Parent == nullptr) {
        start_element->Colour = BLACK;
    }
    
    if (start_element->Colour == RED) {
        if (start_element->Parent != nullptr && start_element->Parent->Colour == RED) {
            if (start_element->Parent->Sibling() != nullptr) {
                if (start_element->Parent->Sibling()->Colour == BLACK) {
                    int rcm = 0;
                    TRBNode* new_centre;
                    if (!start_element->Parent->Isleft()) {
                        if (start_element->Isleft()) {
                            if (start_element->Parent->Parent == root) {
                                rcm = 1;
                            }
                    
                            new_centre = LeftTurn(start_element->Parent);
                            new_centre = RightTurn(start_element->Parent);

                            if (rcm == 1) {
                                root = new_centre;
                                rcm = 0;
                            }
                        }
                        else {
                            if (start_element->Parent->Parent == root) {
                                rcm = 1;
                            }
                            new_centre = RightTurn(start_element->Parent->Parent);

                            if (rcm == 1) {
                                root = new_centre;
                                rcm = 0;
                            }
                        }


                        new_centre->Colour = BLACK;
                        new_centre->Left->Colour = RED;
                        new_centre->Right->Colour = RED;
                    }
                    else {
                        if (!start_element->Isleft()) {
                            if (start_element->Parent->Parent == root) {
                                rcm = 1;
                            }

                            new_centre = RightTurn(start_element->Parent);
                            new_centre = LeftTurn(start_element->Parent);

                            if (rcm == 1) {
                                root = new_centre;
                                rcm = 0;
                            }
                        }
                        else {
                            if (start_element->Parent->Parent == root) {
                                rcm = 1;
                            }
                            new_centre = LeftTurn(start_element->Parent->Parent);

                            if (rcm == 1) {
                                root = new_centre;
                                rcm = 0;
                            }
                        }


                        new_centre->Colour = BLACK;
                        new_centre->Left->Colour = RED;
                        new_centre->Right->Colour = RED;
                    }

                }
                else {
                    start_element->Parent->Parent->Colour = RED;
                    start_element->Parent->Parent->Left->Colour = BLACK;
                    start_element->Parent->Parent->Right->Colour = BLACK;
                    root = AISreballance(start_element->Parent->Parent,root);
                }
            }
            else {
                if (start_element->Parent->Parent == nullptr) {
                    start_element->Parent->Colour = BLACK;
                }
                else {
                    start_element->Parent->Parent->Colour = RED;
                    start_element->Parent->Colour = BLACK;
                }
            }
        }
        else {
            return root;
        }
    }
    else {
        return root;
    }
    return root;
}

int HeigthCounter(TRBNode* start) {
    int counter = 0;
    while (!start->Islist()) {
        if (start->Colour == BLACK) {
            ++counter;
        }
        start = start->Right;
    }

    return counter;
}

int HeigthChecker(TRBNode* start,int maxheigth,int heigth) {
    if (start->Islist() && heigth!=maxheigth) {
        return -1;
    }
    else
    if (!start->Islist() && ((start->Parent != nullptr && !(start->Parent->Right == start || start->Parent->Left== start))||(start->Left->Parent!=start)||(start->Right->Parent!=start))) {
        return -1;
    }
    else {
        int nextheigth = heigth;
        if (start->Colour == BLACK) {
            nextheigth++;
        }

        if (!start->Islist()) {
            if (HeigthChecker(start->Right, maxheigth, nextheigth) != -1 && HeigthChecker(start->Left, maxheigth, nextheigth) != -1) {
                return 0;
            }
            else {
                return -1; 
            }
        }
        else {
            return 0;
        }
    }
}

TRBNode* LoadNode(std::ifstream& fin,TRBNode* pst) {
    int siz[1]; siz[0] = 0;
    unsigned long long val[1]; val[0] = 0;
    char ch[1]; ch[0] = 0;
    try {
        fin.read((char*)&siz[0], sizeof(int) / sizeof(char));
        if(siz[0]<0||siz[0]>257) {
            throw -1;
        }

        char* stri = (char*)calloc(siz[0]+1,sizeof(char));
        if(stri == nullptr) {
            throw -1;
        }
    
        fin.read(&stri[0], siz[0]);

        stri[siz[0]] = 0;
        fin.read((char*)&val[0], sizeof(unsigned long long) / sizeof(char));
        fin.read(&ch[0], 1);
        TRBNode *ND = new(TRBNode);

        if(ND->Key == nullptr) {
            free(stri);
            delete ND;
            throw -1;
        }

        ND->Key = strcpy(ND->Key,stri);
        free(stri);
        ND->Value = val[0];
        ND->Colour = ch[0];
        ND->Parent = pst;

        if (siz[0] == 0) {
            delete ND;
            return nullptr;
        }
        return ND;
    }
    catch (std::ios_base::failure& fail) {
        throw -1; 
    }
    catch (std::bad_alloc()) {
        throw -1;
    }
}

void DestroyNode(TRBNode* dyb) {
    delete dyb;
}

void DestroyTree(TRBNode* root) {
    if (root != nullptr) {
        if (root->Left != nullptr) {
            DestroyTree(root->Left);
            root->Left = nullptr;
        }
        if (root->Right != nullptr) {
            DestroyTree(root->Right);
            root->Right = nullptr;
        }

        DestroyNode(root);
    }
}


TRBNode* LoadTree(std::ifstream& fin, TRBNode* root) {
    TRBNode* croot;
    try {
        croot = LoadNode(fin, root);
    }
    catch (int err) {
        return nullptr;
    }
    if (croot == nullptr) {
        try {
            croot = new(TRBNode);
        } 
        catch (std::bad_alloc()) {
            return nullptr;
        }

        if(croot->Key == nullptr) {
            delete croot;
            return nullptr;
        }

        croot->Key = strcpy(croot->Key,"");
        croot->Value = 0;
        croot->Colour = BLACK;
        croot->Parent = root;
        croot->Left = nullptr;
        croot->Right = nullptr;
        return croot;
    }

    croot->Left = LoadTree(fin, croot);
    croot->Right = LoadTree(fin, croot);

    if(croot->Left == nullptr || croot->Right == nullptr) {

        if(croot->Left != nullptr) {
            DestroyTree(croot->Left);
            croot->Left = nullptr;
        }

        if(croot->Right != nullptr) {
            DestroyTree(croot->Right);
            croot->Right = nullptr;
        }

        delete croot;
        return nullptr;
    }

    return croot;
}

int SaveNode(std::ofstream& fout, TRBNode* ldb) {
    try {
        int size = strlen(ldb->Key);
        fout.write((char*)&size, sizeof(size));
        fout.write(ldb->Key, size);
        fout.write((char*)&ldb->Value, sizeof(unsigned long long));
        fout.write(&ldb->Colour, 1);
    }
    catch (std::ios_base::failure& fail) {
        return -1;
    }

    return 0;
}

int SaveTree(std::ofstream& fout, TRBNode* root) {
    if (root == nullptr) {
        return 0;
    }
    if (SaveNode(fout, root) != 0) {
        return -1;
    }
    if (root->Left != nullptr) {
        if (SaveTree(fout, root->Left) != 0) {
            return -1;
        }
    }
    if (root->Right != nullptr) {
        if (SaveTree(fout, root->Right) != 0) {
            return -1;
        }
    }

    return 0;
}

class TRBTree {
public:

    ~TRBTree () {
        DestroyTree(TreeRoot);
    }

    int Add(char* key,unsigned long long& value){
        if(key == nullptr ) {
            return -3;
        }

        if (TreeRoot == nullptr) {
            try {
                TreeRoot = new(TRBNode);
            }
            catch(std::bad_alloc()) {
                TreeRoot = nullptr; 
                return -1;
            }

            if(TreeRoot->Key == nullptr) {
                TreeRoot = nullptr; 
                return -1;
            }

            TreeRoot->Colour = BLACK;
            TreeRoot->Parent = nullptr;
            TreeRoot->Left = nullptr;
            TreeRoot->Right = nullptr;

            TreeRoot->Key = strcpy(TreeRoot->Key,key);
            TreeRoot->Value = value;

            try {
                TreeRoot->Right = new(TRBNode);
            }
            catch(std::bad_alloc()) {
                delete TreeRoot;
                TreeRoot = nullptr; 
                return -1;
            }

            if (TreeRoot->Right->Key == nullptr) {
                delete TreeRoot->Right;
                delete TreeRoot;
                TreeRoot = nullptr; 
                return -1;
            }

            TreeRoot->Right->Parent = TreeRoot;

            try {
                TreeRoot->Left = new(TRBNode);
            }
            catch(std::bad_alloc()) {
                delete TreeRoot->Right;
                delete TreeRoot;
                TreeRoot = nullptr; 
                return -1;
            }

            if (TreeRoot->Left->Key == nullptr) {
                delete TreeRoot->Right;
                delete TreeRoot->Left;
                delete TreeRoot;
                TreeRoot = nullptr; 
                return -1;
            }

            TreeRoot->Left->Parent = TreeRoot;
        }
        else {
            TRBNode* last = nullptr;
            TRBNode* cur = TreeRoot;
            //поиск места для вставки
            while (!cur->Islist())
            {
                if(cur->Key == nullptr) {
                    return -4;
                }

                if (strequal(key, cur->Key) == 0) {
                    return -7;
                }
                if (strequal(key, cur->Key) > 0) {
                    last = cur;
                    cur = cur->Right;
                }
                else
                if (strequal(key, cur->Key) < 0) {
                    last = cur;
                    cur = cur->Left;
                }
            }
            cur->Parent = last; 
            cur->Key = strcpy(cur->Key,key);
            cur->Value = value;
            cur->Colour = RED;

            try {    
                cur->Right = new TRBNode;
            }
            catch (std::bad_alloc()) {
                cur->Key = strcpy(cur->Key,"");
                cur->Value = 0;
                cur->Colour = BLACK;
                cur->Right = nullptr;
                cur->Left = nullptr;
                return -1;
            }

            try {    
                cur->Left = new TRBNode;
            }
            catch (std::bad_alloc()) {
                delete cur->Right;
                cur->Key = strcpy(cur->Key,"");
                cur->Value = 0;
                cur->Colour = BLACK;
                cur->Right = nullptr;
                cur->Left = nullptr;
                return -1;
            }

            if(cur->Right->Key == nullptr || cur->Left->Key == nullptr) {
                
                if(cur->Left != nullptr) {
                    delete cur->Left;
                }

                if(cur->Right != nullptr) {
                    delete cur->Right;
                }

                cur->Key = strcpy(cur->Key,"");
                cur->Value = 0;
                cur->Colour = BLACK;
                cur->Right = nullptr;
                cur->Left = nullptr;
                return -1;
            }

            cur->Right->Parent = cur;
            cur->Left->Parent = cur;

            TreeRoot = AISreballance(cur,TreeRoot);
        }

        return 0;
    }

    int Remove(char* inpkey) {
        if(inpkey == nullptr) {
            return -8;
        }
        TRBNode* cur = TreeRoot;
        if (cur == nullptr) {
            return -8;
        }

        while (true) {
            if (cur->Key == nullptr) {
                return -1;
            }
            if (cur->Islist()) {
                return -8;
            }

            if (strequal(inpkey, cur->Key) == 0) {
                break;
            }
            else
            if (strequal(inpkey, cur->Key) > 0) {
                cur = cur->Right;
            }
            else
            if (strequal(inpkey, cur->Key) < 0) {
                cur = cur->Left;
            }
        }


        if (cur->Left->Islist() && cur->Right->Islist()) {
            if (cur == TreeRoot) {
                TreeRoot = nullptr;
                delete cur->Left;
                delete cur->Right;
                delete cur;

                return 0;
            }
            else
            if (cur->Colour == RED) {
                if (cur->Isleft()) {
                    cur->Left->Parent = cur->Parent;
                    cur->Parent->Left = cur->Left;
                }
                else {
                    cur->Left->Parent = cur->Parent;
                    cur->Parent->Right = cur->Left;
                }
                delete cur->Right;
                delete cur;
            }
            else {
                if (cur->Isleft()) {
                    if(cur->Parent!=nullptr)
                        cur->Parent->Left = cur->Left;

                    cur->Left->Parent = cur->Parent;
                    TRBNode* start = cur->Left;
                    delete cur->Right;
                    delete cur;
                    TreeRoot = ARMreballance(start, TreeRoot);
                }
                else {
                    if (cur->Parent != nullptr)
                        cur->Parent->Right = cur->Left;

                    cur->Left->Parent = cur->Parent;
                    TRBNode* start = cur->Left;
                    delete cur->Right;
                    delete cur;
                    TreeRoot = ARMreballance(start, TreeRoot);
                }
            }
        }
        else
        if (cur->Left->Islist()) {

            if (cur == TreeRoot) {
                TreeRoot = cur->Right;
                TreeRoot->Colour = BLACK;
                TreeRoot->Parent = nullptr;
                delete cur->Left;
                delete cur;

                return 0;
            }

            if (cur->Colour == RED) {
                if (cur->Isleft()) {
                    cur->Right->Parent = cur->Parent;
                    cur->Parent->Left = cur->Right;
                }
                else {
                    cur->Right->Parent = cur->Parent;
                    cur->Parent->Right = cur->Right;
                }
                delete cur->Left;
                delete cur;
            }
            else {
                if (cur->Right->Colour == RED) {
                    if (cur->Isleft()) {
                        cur->Right->Parent = cur->Parent;
                        cur->Right->Colour = BLACK;
                        cur->Parent->Left = cur->Right;
                    }
                    else {
                        cur->Right->Parent = cur->Parent;
                        cur->Right->Colour = BLACK;
                        cur->Parent->Right = cur->Right;
                    }
                    delete cur->Left;
                    delete cur;
                }
                else {
                    return -2;
                }
            }
        }
        else
        if (cur->Right->Islist()) {

            if (cur == TreeRoot) {
                TreeRoot = cur->Left;
                TreeRoot->Colour = BLACK;
                TreeRoot->Parent = nullptr;
                delete cur->Right;
                delete cur;

                return 0;
            }

            if (cur->Colour == RED) {
                if (cur->Isleft()) {
                    cur->Left->Parent = cur->Parent;
                    cur->Parent->Left = cur->Left;
                }
                else {
                    cur->Left->Parent = cur->Parent;
                    cur->Parent->Right = cur->Left;
                }
                delete cur->Right;
                delete cur;
            }
            else {
                if (cur->Left->Colour == RED) {
                    if (cur->Isleft()) {
                        cur->Left->Parent = cur->Parent;
                        cur->Left->Colour = BLACK;
                        cur->Parent->Left = cur->Left;
                    }
                    else {
                        cur->Left->Parent = cur->Parent;
                        cur->Left->Colour = BLACK;
                        cur->Parent->Right = cur->Left;
                    }
                    delete cur->Right;
                    delete cur;
                }
                else {
                    return -2;
                }
            }
        }
        else {
            TRBNode* cnb = cur->Right;
            while (!cnb->Left->Islist()) {
                cnb = cnb->Left;
            }
            
            try {
                Swap(cur, cnb);
            }
            catch (int err) {
                return -1;
            }
            cur = cnb;
            if (cur->Left->Islist() && cur->Right->Islist()) {
                if (cur == TreeRoot) {
                    TreeRoot = nullptr;
                    delete cur->Left;
                    delete cur->Right;
                    delete cur;

                    return 0;
                }
                else
                if (cur->Colour == RED) {
                    if (cur->Isleft()) {
                        cur->Left->Parent = cur->Parent;
                        cur->Parent->Left = cur->Left;
                    }
                    else {
                        cur->Left->Parent = cur->Parent;
                        cur->Parent->Right = cur->Left;
                    }
                    delete cur->Right;
                    delete cur;
                }
                else {
                    if (cur->Isleft()) {
                        if(cur->Parent!=nullptr)
                            cur->Parent->Left = cur->Left;

                        cur->Left->Parent = cur->Parent;
                        TRBNode* start = cur->Left;
                        delete cur->Right;
                        delete cur;
                        TreeRoot = ARMreballance(start, TreeRoot);
                    }
                    else {
                        if (cur->Parent != nullptr)
                            cur->Parent->Right = cur->Left;

                        cur->Left->Parent = cur->Parent;
                        TRBNode* start = cur->Left;
                        delete cur->Right;
                        delete cur;
                        TreeRoot = ARMreballance(start, TreeRoot);
                    }
                }
            }
            else
            if (cur->Left->Islist()) {

                if (cur == TreeRoot) {
                    TreeRoot = cur->Right;
                    TreeRoot->Colour = BLACK;
                    TreeRoot->Parent = nullptr;
                    delete cur->Left;
                    delete cur;

                    return 0;
                }

                if (cur->Colour == RED) {
                    if (cur->Isleft()) {
                        cur->Right->Parent = cur->Parent;
                        cur->Parent->Left = cur->Right;
                    }
                    else {
                        cur->Right->Parent = cur->Parent;
                        cur->Parent->Right = cur->Right;
                    }
                    delete cur->Left;
                    delete cur;
                }
                else {
                    if (cur->Right->Colour == RED) {
                        if (cur->Isleft()) {
                            cur->Right->Parent = cur->Parent;
                            cur->Right->Colour = BLACK;
                            cur->Parent->Left = cur->Right;
                        }
                        else {
                            cur->Right->Parent = cur->Parent;
                            cur->Right->Colour = BLACK;
                            cur->Parent->Right = cur->Right;
                        }
                        delete cur->Left;
                        delete cur;
                    }
                    else {
                        return -2;
                    }
                }
            }
            else
            if (cur->Right->Islist()) {

                if (cur == TreeRoot) {
                    TreeRoot = cur->Left;
                    TreeRoot->Colour = BLACK;
                    TreeRoot->Parent = nullptr;
                    delete cur->Right;
                    delete cur;

                    return 0;
                }

                if (cur->Colour == RED) {
                    if (cur->Isleft()) {
                        cur->Left->Parent = cur->Parent;
                        cur->Parent->Left = cur->Left;
                    }
                    else {
                        cur->Left->Parent = cur->Parent;
                        cur->Parent->Right = cur->Left;
                    }
                    delete cur->Right;
                    delete cur;
                }
                else {
                    if (cur->Left->Colour == RED) {
                        if (cur->Isleft()) {
                            cur->Left->Parent = cur->Parent;
                            cur->Left->Colour = BLACK;
                            cur->Parent->Left = cur->Left;
                        }
                        else {
                            cur->Left->Parent = cur->Parent;
                            cur->Left->Colour = BLACK;
                            cur->Parent->Right = cur->Left;
                        }
                        delete cur->Right;
                        delete cur;
                    }
                    else {
                        return -2;
                    }
                }
            }

        }

        return 0;
    }

    TRBNode* Max() {
        TRBNode* scb = TreeRoot;
        if (scb == nullptr) {
            return nullptr;
        }

        while (!scb->Right->Islist()) {
            scb = scb->Right;
        }

        return scb;
    }

    TRBNode* Min() {
        TRBNode* scb = TreeRoot;
        if (scb == nullptr) {
            return nullptr;
        }

        while (!scb->Left->Islist()) {
            scb = scb->Left;
        }

        return scb;
    }

    TRBNode* Root() {
        return TreeRoot;
    }

    void Print() {
        std::cout << "print started\n";
        Tree_print(TreeRoot,0);
        std::cout << "print finished\n";
    }

    int ChekBlackHeighth()
    {
        if (TreeRoot == nullptr) {
            return 0;
        }
        if (HeigthChecker(TreeRoot, HeigthCounter(TreeRoot), 0) == -1) {
            return -1;
        }
        else {
            return 0;
        }
    }

    TRBNode* Find(char* inpkey) {
        if (inpkey == nullptr) {
            return nullptr;
        }
        TRBNode* cur = TreeRoot; 
        if(cur == nullptr) {
            return nullptr;
        }

        while (true) {
            if (cur->Right == nullptr || cur->Left == nullptr) {
                return nullptr;
            }
            if (cur->Key == nullptr) {
                return nullptr;
            }

            if (strequal(inpkey, cur->Key) == 0) {
                break;
            }
            else
            if (strequal(inpkey, cur->Key) > 0) {
                //last = cur;
                cur = cur->Right;
            }
            else
            if (strequal(inpkey, cur->Key) < 0) {
                //last = cur;
                cur = cur->Left;
            }
        }

        return cur;
    }

    int SaveToDisk(const std::string& path) {
        try {
            std::ofstream fout;
            fout.open(path, std::ios::binary | std::ios::out);
            if(!fout.is_open()) {
                
                return -1;
            }
            char mark[3] = { 'r','b','t' };
            fout.write((char*)&mark[0], sizeof(char) * 3);
            if (SaveTree(fout, TreeRoot) != 0) {
                fout.close();
                return -2;
            }
            fout.close();
        }
        catch (std::ios_base::failure& fail) {
            return -3;
        }

        return 0;
    }

    int LoadFromDisk(const std::string& path) {

        char mark[4];
        std::ifstream fin;
        try {
            fin = std::ifstream(path, std::ios::binary | std::ios::in);
            if(!fin.is_open()) {
                return -3;
            }
            fin.read((char*)mark, sizeof(char) * 3);
        }
        catch (std::ios_base::failure& fail) {
            fin.close();
            return -3;
        }

        if (!(mark[0] == 'r' && mark[1] == 'b' && mark[2] == 't'))
        {
            fin.close();
            return -2;
        }

        TRBNode *NewRoot = nullptr;
        NewRoot = LoadTree(fin, NewRoot);
        if (NewRoot != nullptr) {

            fin.close();
            DestroyTree(TreeRoot);
            TreeRoot = NewRoot;
            return 0;
        }

        DestroyTree(NewRoot);
        fin.close();
        return -1;
    }

    void Destroy() {
        DestroyTree(TreeRoot);
        TreeRoot = nullptr;
    }

private:
    TRBNode* TreeRoot = nullptr;
};




int main() {
    TRBTree maintree; 

    char *inpstr = (char*)calloc(260,sizeof(char)); 
    char *word = (char*)calloc(260,sizeof(char)); 
    unsigned long long inpval;

    if(inpstr == NULL || inpstr == nullptr || word == NULL || word == nullptr) {
        printf("ERROR: allocation error\n");
        return -1;
    }

    while (scanf("%s", inpstr) != EOF) {
        if(strlen(inpstr)>256) {
            printf("ERROR: uncorrect input\n");
            continue;
        }
        if (inpstr[0] == '+') {
            if(scanf("%s %llu", word, &inpval) == EOF) {
                break;
            }
            if(strlen(word)>256) {
                printf("ERROR: uncorrect input\n");
                continue;
            }
            int mrk = maintree.Add(word, inpval);
            if (mrk == 0) {
                printf("OK\n");
            }
            else {
                if (mrk == -7) {
                    printf("Exist\n");
                }
                else
                if (mrk == -3) {
                    printf("ERROR: empty input\n");
                }
                else
                if (mrk == -4) {
                    printf("ERROR: untraced allocation error\n");
                }
                else
                if (mrk == -1) {
                    printf("ERROR: out of memory\n");
                }
                else {
                    printf("ERROR: unknown error\n");
                }
            }
        }
        else 
        if (inpstr[0] == '-') {
            if(scanf("%s", word) == EOF) {
                break;
            }
            if(strlen(word)>256) {
                printf("ERROR: uncorrect input\n");
                continue;
            }
            int mrk = maintree.Remove(word);
            if (mrk == 0) {
                printf("OK\n");
            }
            else 
            if (mrk == -8) {
                printf("NoSuchWord\n");
            }
            else
            if (mrk == -1) {
                printf("ERROR: out of memory\n");
            }
            else {
                printf("ERROR: unknown error\n");
            }
        }
        else
        if (inpstr[0] == '!') {
            std::string path;
            if(scanf("%s", word) == EOF) {
                break;
            }
            if (strcmp(word, "Save") == 0) {
                std::cin >> path;
                
                int mrk = maintree.SaveToDisk(path); 
                if (mrk == 0) {
                    printf("OK\n");
                    continue;
                }
                else 
                if (mrk == 1) {
                    printf("OK\n");
                    continue;
                }
                else 
                if (mrk == -1) {
                    printf("ERROR: unable to open file\n");
                    continue;
                }
                else 
                if (mrk == -2) {
                    printf("ERROR: unable to write file\n");
                    continue;
                }
                else 
                if (mrk == -3) {
                    printf("ERROR: file acsess error\n");
                    continue;
                }
                else 
                {
                    printf("ERROR: something gone wrong\n");
                    continue;
                }
            }
            else
            if (strcmp(word, "Load") == 0) {
                std::cin >> path;

                int mrk = maintree.LoadFromDisk(path); 
                if (mrk == 0) {
                    printf("OK\n");
                    continue;
                }
                else 
                if (mrk == -1) {
                    printf("ERROR: file is damaged\n");
                    continue;
                }
                else 
                if (mrk == -2) {
                    printf("ERROR: wrong format of file\n");
                    continue;
                }
                else 
                if (mrk == -3) {
                    printf("ERROR: file acsess error\n");
                    continue;
                }
                else 
                {
                    printf("ERROR: something gone wrong\n");
                    continue;
                }
            }
            
        }
        else {
            TRBNode* res = maintree.Find(inpstr);
            if (res != nullptr) {
                printf("OK: %llu\n",res->Value);
            }
            else {
                printf("NoSuchWord\n");
            }
        }
    }
    
    maintree.Destroy(); 
    free(inpstr);
    free(word);

    return 0;
}