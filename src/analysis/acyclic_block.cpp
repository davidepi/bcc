//
// Created by davide on 7/5/19.
//

#include "acyclic_block.hpp"
#include "basic_block.hpp"
#include <cassert>
#include <iostream>
#include <stack>

// The SequenceBlock::delete_list containg elements on which `delete` should be
// called. This because if the components of the sequence are other sequences,
// they are flattened. But they still have the ownership of the contained
// elements and I cannot remove the ownership without violating the const-ness
// (thus modifying the flattened sequence).

SequenceBlock::SequenceBlock(int id, const AbstractBlock* fst,
                             const AbstractBlock* snd)
    : AbstractBlock(id)
{
    auto merge_blocks = [this](const AbstractBlock* p) -> void {
        // merge all the internals of a sequence, and destroy the sequence
        if(p->get_type() == BlockType::SEQUENCE)
        {
            int size = p->size();
            for(int i = 0; i < size; i++)
            {
                components.push_back((*p)[i]);
            }
        }
        // if it was a single node just add the node
        else
        {
            components.push_back(p);
        }
        delete_list.push_back(p);
    };
    merge_blocks(fst);
    merge_blocks(snd);
}

BlockType SequenceBlock::get_type() const
{
    return SEQUENCE;
}

SequenceBlock::~SequenceBlock()
{
    for(const AbstractBlock* block : delete_list)
    {
        delete block;
    }
}

int SequenceBlock::size() const
{
    return components.size();
}

const AbstractBlock* SequenceBlock::operator[](int index) const
{
    return components[index];
}

BlockType IfThenBlock::get_type() const
{
    return IF_THEN;
}

IfThenBlock::IfThenBlock(int id, const BasicBlock* ifb,
                         const AbstractBlock* thenb)
    : AbstractBlock(id), head(ifb), then(thenb)
{
}

IfThenBlock::~IfThenBlock()
{
    delete head;
    delete then;
}

int IfThenBlock::size() const
{
    return 2;
}

const AbstractBlock* IfThenBlock::operator[](int index) const
{
    return index == 0 ? head : then;
}

IfElseBlock::IfElseBlock(int id, const BasicBlock* ifb,
                         const AbstractBlock* thenb, const AbstractBlock* elseb)
    : AbstractBlock(id), head(ifb), then(thenb), ellse(elseb)
{
    // resolve chained heads
    std::stack<const BasicBlock*> chain_stack;
    const BasicBlock* tmp_head = ifb;
    const AbstractBlock* next = tmp_head->get_next() != elseb ?
                                    tmp_head->get_next() :
                                    tmp_head->get_cond();
    while(next != thenb)
    {
        chain_len++;
        tmp_head = static_cast<const BasicBlock*>(next);
        chain_stack.push(tmp_head);
        next = tmp_head->get_next() != elseb ? tmp_head->get_next() :
                                               tmp_head->get_cond();
    }

    if(chain_len != 0)
    {
        // copy the stack into the more space_efficient array
        chain =
            (const BasicBlock**)malloc(sizeof(const BasicBlock*) * chain_len);
        for(int i = chain_len - 1; i >= 0; i--)
        {
            chain[i] = chain_stack.top();
            chain_stack.pop();
        }
    }
    else
    {
        chain = nullptr;
    }
}

IfElseBlock::~IfElseBlock()
{
    delete ellse;
    delete then;
    delete head;
    if(chain != nullptr)
    {
        for(int i = 0; i < chain_len; i++)
        {
            delete chain[i];
        }
        free(chain);
    }
}

BlockType IfElseBlock::get_type() const
{
    return BlockType::IF_ELSE;
}

int IfElseBlock::size() const
{
    return chain_len + 3;
}

const AbstractBlock* IfElseBlock::operator[](int index) const
{
    if(index == 0)
    {
        return head;
    }
    else if(index == 1)
    {
        return then;
    }
    else if(index == 2)
    {
        return ellse;
    }
    else
    {
        return chain[index - 3];
    }
}