#include "GeneModelSaver.hpp"

using namespace std;

GeneModelSaver::GeneModelSaver(std::shared_ptr<Genome> g)
{
    this->genome = g;
}

GeneModelSaver::~GeneModelSaver()
{
}

void GeneModelSaver::Save(string &filename)
{
    char *cstr = new char[filename.length() + 1];
    strcpy(cstr, filename.c_str());
    fstream fp;
    fp.open(cstr, ios::out); //開啟檔案
    if (!fp)
    { //如果開啟檔案失敗，fp為0；成功，fp為非0
        cout << "Fail to open file: " << cstr << endl;
    }

    fp << this->genome->genomme_id << " " << this->genome->nodes.size() << " " << this->genome->links.size() << endl;

    for (auto const &node : this->genome->nodes)
    {
        fp << node->getNodeId() << " " << node->getNodeType() << endl;
    }

    for (auto const &link : this->genome->links)
    {
        fp << link->getInNodeId() << " " << link->getOutNodeId() << endl;
    }

    // cout << "File Descriptor: " << fp << endl;
    // fp << "Hello HappyMan!!" << endl; //寫入字串

    fp.close(); //關閉檔案
}
