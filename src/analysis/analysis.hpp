//
// Created by davide on 6/12/19.
//

#ifndef __ANALYSIS_HPP__
#define __ANALYSIS_HPP__

#include "architectures/architecture.hpp"
#include "basic_block.hpp"
#include "disassembler/statement.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * \brief Class used to perform the analysis of the disassembled code
 *
 * TODO: write desc
 */
class Analysis
{
public:
    /**
     * \brief Constructor given a vector of Statement
     *
     * This method is used to run the analysis on the resulting method obtained
     * after calling the Disassembler analyse() function
     *
     * \param[in] stmts A vector of Statement, likely obtained from the
     * Disassembler class
     * \param[in] arch A pointer to the Architecture class representing the
     * binary architecture. This pointer is not inherited but will be used
     * during the class lifecycle
     */
    Analysis(const std::vector<Statement>* stmts,
             std::shared_ptr<Architecture> arch);

    /**
     * \brief Constructor given a string representing a function
     *
     * This method is used to perform the analysis on a generic string obtained
     * by other means. A string should represent an entire function. The syntax
     * of the string is expected as follow: <ul> <li> The first line MUST
     * contain everything but a statement (because it is skipped since usually I
     * put the function name)</li> <li> Each line representing a statement with
     * the following syntax: <ol><li>The offset of the statement, either in
     * hexadecimal (prepended by 0x or 0X, who cares about case) or decimal
     * form</li> <li> A single space </li><li>The instruction represented as
     * string</li></ol></li></ul>
     *
     * The string will be automatically converted to lowercase
     *
     * \param[in] str A string representing a single function, formatted as
     * described in the doc
     * \param[in] arch A pointer to the Architecture class representing the
     * binary architecture. This pointer is not inherited but will be used
     * during the class lifecycle
     */
    Analysis(const std::string& str, std::shared_ptr<Architecture> arch);

    /**
     * \brief Default destructor
     */
    virtual ~Analysis();

    //TODO: deal with rule of the 3

    /**
     * \brief Access the n-th instruction
     *
     * If the instruction does not exists (out of bounds), an empty Statement
     * will be returned
     *
     * \note This function expect an index, and not an offset as parameter! So
     * the first instruction can be found with the value 0, not with its offset
     * in the program
     *
     * \param[in] value The index of the instruction. Not the offset!
     * \return The instruction found at the given index
     */
    Statement operator[](unsigned int value) const;

    /**
     * \brief Return the control flow graph for this function
     * \return the control flow graph of the function
     */
    const BasicBlock* get_cfg() const;

private:
    // build a control flow graph in O(nlogn) time-complexity
    void build_cfg();

    // linear;y stored instructions
    std::vector<Statement> stmt_list;

    // sparsely stored instructions, indexed by offset
    std::unordered_map<uint64_t, const Statement*> stmt_sparse;

    // class used to gather architecture specific information
    std::shared_ptr<Architecture> architecture;

    // control flow graph of the function
    BasicBlock* cfg;
};

#endif //__ANALYSIS_HPP__