#include <iostream>
#include <string>

#include "p3.h"

using namespace std;

Info::Info() {
    this->name = "No Name Set";
    this->value = "No Value Set";
    this->next = nullptr;
}

Info::Info(std::string name, std::string value, Info *next) {
    this->name = name;
    this->value = value;
    this->next = next;
}

Contact::Contact() {
    this->first = "No First Set";
    this->last = "No Last Set";
    this->next = nullptr;
    this->headInfoList = nullptr;
}

Contact::Contact(std::string first, std::string last, Contact *next) {
    this->first = first;
    this->last = last;
    this->next = next;
    this->headInfoList = nullptr;
}

ContactList::ContactList() {
    this->headContactList = nullptr;
    this->count = 0;
}

int ContactList::getCount() {
    return this->count;
}

// 1. return false and print nothing if the contact is not in the list
// 2. otherwise return true and print the contact
bool ContactList::printContact(std::ostream &os, std::string first, std::string last) {
    Contact* curNode = this->headContactList;
    while(curNode)
    {
        if(curNode->first == first && curNode->last == last)
        {
            os << "Contact Name: " << curNode->first << " " << curNode->last << endl;
            Info* nodeI = curNode->headInfoList;
            while(nodeI)
            {
                os << "\t" << nodeI->name << " | " << nodeI->value << endl;
                nodeI = nodeI->next;
            }
            return true;
        }
        curNode = curNode->next;
    }
    return false;
}

// print all contacts and their information
// print nothing if the list is empty
void ContactList::print(std::ostream &os) {
    Contact* node = this->headContactList;
    while(node)
    {
        printContact(os, node->first, node->last);
        node = node->next;
    }
}

// add a contact to the back of the list
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the back of the list
// - do not forget to update count
bool ContactList::addContact(std::string first, std::string last) {
    Contact* insertNode = new Contact(first, last);
    if(!this->headContactList)
    {
        this->headContactList = insertNode;
        this->count++;
        return true;
    }
    Contact* testNode = this->headContactList;
    while(testNode->next)
    {
        if(testNode->first == first && testNode->last == last)
        {
            delete insertNode;
            return false;
        }
        testNode = testNode->next;
    }
    if(testNode->first == first && testNode->last == last)
    {
        delete insertNode;
        return false;
    }
    testNode->next = insertNode;
    this->count++;
    return true;
}

// add info to the back of a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the back of the contact's list and return true
bool ContactList::addInfo(std::string first, std::string last, std::string infoName, std::string infoVal) {
    Info* insertNode = new Info(infoName, infoVal);
    Contact* curNodeC = this->headContactList;
    while(curNodeC)
    {
        if(curNodeC->first == first && curNodeC->last == last)
        {
            if(!curNodeC->headInfoList)
            {
                curNodeC->headInfoList = insertNode;
                return true;
            }
            Info* curNodeI = curNodeC->headInfoList;
            while (curNodeI)
            {
                if(curNodeI->name == infoName)
                {
                    curNodeI->value = infoVal;
                    delete insertNode;
                    return true;
                }
                if(!curNodeI->next)
                {
                    curNodeI->next = insertNode;
                    return true;
                }
                curNodeI = curNodeI->next;
            }
        }
        curNodeC = curNodeC->next;
    }
    delete insertNode;
    return false;
}

// add a contact to the list in ascending order by last name
//     if last names are equal, then order by first name ascending
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the list
// - do not forget to update count
// - compare strings with the built-in comparison operators: <, >, ==, etc.
// - a compare method/function is recommended
bool ContactList::addContactOrdered(std::string first, std::string last) {
    Contact* insertNode = new Contact(first, last);
    Contact* curNode = this->headContactList;
    Contact* prevNode = nullptr;
    if(!this->headContactList) 
    {
        this->headContactList = insertNode;
        this->count++;
        return true;
    }
    while(curNode)  
    {
        if(curNode->first == first && curNode->last == last)    
        {
            delete insertNode;
            return false;
        }
        curNode = curNode->next;
    }
    curNode = this->headContactList;
    while(curNode && (curNode->last < insertNode->last || (curNode->last == insertNode->last && curNode->first < insertNode->first)))
    {
        prevNode = curNode;
        curNode = curNode->next;
    }
    if(prevNode)
    {
        prevNode->next = insertNode;
    }
    else
    {
        this->headContactList = insertNode;
    }
    insertNode->next = curNode;
    this->count++;
    return true;      
}

// add info to a contact's info list in ascending order by infoName
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the contact's list and return true
bool ContactList::addInfoOrdered(std::string first, std::string last, std::string infoName, std::string infoVal) {
    Info* insertNode = new Info(infoName, infoVal);
    Contact* curCon = this->headContactList;
    if(this->headContactList == nullptr)
    {
        delete insertNode;
        return false;
    }
    while(curCon)
    {
        if(curCon->first == first && curCon->last == last)
        {
            Info* curInf = curCon->headInfoList;
            Info* prevInf = nullptr;
            if(!curCon->headInfoList)
            {
                curCon->headInfoList = insertNode;
                return true;
            }
            while(curInf)
            {
                if(curInf->name == infoName)
                {
                    delete insertNode;
                    curInf->value = infoVal;
                    return true;
                }
                if(curInf->name > infoName)
                {
                    break;
                }
                prevInf = curInf;
                curInf = curInf->next;
            }
            if(prevInf)
            {
                prevInf->next = insertNode;
            }
            else
            {
                curCon->headInfoList = insertNode;
            }
            insertNode->next = curInf;
            return true;
        }
        curCon = curCon->next;
    }
    delete insertNode;
    return false;
}

// remove the contact and its info from the list
// 1. return false and do nothing if the contact is not in the list
// 2. otherwise return true and remove the contact and its info
// - do not forget to update count
bool ContactList::removeContact(std::string first, std::string last) {
    Contact* temp = nullptr;
    if(!this->headContactList)
    {
        return false;
    }
    if(this->headContactList->first == first && this->headContactList->last == last)
    {
        temp = this->headContactList;
        this->headContactList = this->headContactList->next;
        delete temp;
        count--;
        return true;
    }
    Contact* curCon = this->headContactList;
    while(curCon->next)
    {
        if(curCon->next->first == first && curCon->next->last == last)
        {
            temp = curCon->next;
            if(temp->next == nullptr)
            {
                curCon->next = nullptr;
                delete temp;
                this->count--;
                return true;
            }
            curCon->next = temp->next;
            delete temp;
            this->count--;
            return true;
        }
        curCon = curCon->next;
    }
    return false;
}

// remove the info from a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. return false and do nothing if the info is not in the contact's info list
// 3. otherwise return true and remove the info from the contact's list
bool ContactList::removeInfo(std::string first, std::string last, std::string infoName) {
    Info* temp = nullptr;
    if(!this->headContactList)
    {
        return false;
    }
    Contact* curCon = this->headContactList;
    while(curCon)
    {
        if(curCon->first == first && curCon->last == last)
        {
            if(!curCon->headInfoList)
            {
                return false;
            }
            if(curCon->headInfoList->name == infoName)
            {
                temp = curCon->headInfoList;
                curCon->headInfoList = curCon->headInfoList->next;
                delete temp;
                return true;
            }
            Info* curInf = curCon->headInfoList;
            while(curInf->next)
            {
                if(curInf->next->name == infoName)
                {
                    temp = curInf->next;
                    if(temp->next == nullptr)
                    {
                        curInf->next = nullptr;
                        delete temp;
                        return true;
                    }
                    curInf->next = temp->next;
                    delete temp;
                    return true;
                }
                curInf = curInf->next;
            }
            return false;
        }
        curCon = curCon->next;
    }
    return false;
}

// destroy the list by removing all contacts and their infos
ContactList::~ContactList() {
    while(this->headContactList)
    {
        Contact* tempNode = this->headContactList;
        while(tempNode->headInfoList)
        {
            Info* tempInfo = tempNode->headInfoList;
            tempNode->headInfoList = tempNode->headInfoList->next;
            delete tempInfo;
        }
        this->headContactList = this->headContactList->next;
        delete tempNode;
    }
}

// deep copy the source list
// - do not forget to update count
ContactList::ContactList(const ContactList &src) {
    this->count = 0;
    this->headContactList = nullptr;
    if(src.headContactList != nullptr)
    {
        Contact* srcCon = src.headContactList;
        while(srcCon)
        {
            this->addContact(srcCon->first, srcCon->last);
            Info* srcInf = srcCon->headInfoList;
            while(srcInf)
            {
                this->addInfo(srcCon->first, srcCon->last, srcInf->name, srcInf->value);
                srcInf = srcInf->next;
            }
            srcCon = srcCon->next;
        }
    }
}

// remove all contacts and their info then deep copy the source list
// - do not forget to update count

const ContactList &ContactList::operator=(const ContactList &src) {
    if (this != &src) {
        delete this;
        Contact* node = src.headContactList;
        while(node)
        {
            this->addContact(node->first, node->last);
            Info* nodeI = node->headInfoList;
            while(nodeI)
            {
                this->addInfo(node->first, node->last, nodeI->name, nodeI->value);
                nodeI = nodeI->next;
            }
            node = node->next;
        }
        this->count = src.count;
    }
    return *this;
}
