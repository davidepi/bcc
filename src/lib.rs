#![allow(clippy::upper_case_acronyms)] // I hate this >:(
#![allow(clippy::comparison_chain)]
// this is ugly and completely unreadable
//EDIT 2021/05/28 this lint would've prevented me a bug :( ----^
#![cfg_attr(docsrs, feature(doc_cfg))]

/// Module containing the analysis to compare clones.
pub mod analysis;
/// Module providing disassembler bindings.
pub mod disasm;
