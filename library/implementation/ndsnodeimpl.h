#ifndef NDSNODEIMPL_H
#define NDSNODEIMPL_H

#include "ndsbaseimpl.h"
#include <list>

namespace nds
{

class Node;

/**
 * @brief Represents a node (channel or channelGroup in the old NDS) which can contain
 *        other nodes.
 *
 * Can contain other nodes (including AsynPort) or PVs.
 */
class NodeImpl: public BaseImpl
{
public:
    /**
     * @brief Construct the node
     *
     * @param name  node's name (e.g. "CHANNEL0" or "CHANNELGROUP1")
     */
    NodeImpl(const std::string& name, Node* pInterface);

    ~NodeImpl();

    void addChild(Base* child);

    virtual void initialize();


private:
    typedef std::map<std::string, Base* > tChildren;
    tChildren m_children;
};

}

#endif // NDSNODEIMPL_H