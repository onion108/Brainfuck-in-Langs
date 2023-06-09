use std::{fmt::Display, error::Error};

#[derive(Debug)]
pub enum CompileError {
    UnmatchedBrackets,
    IOError(std::io::Error)
}

impl Display for CompileError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            &Self::UnmatchedBrackets => write!(f, "Unmatched brackets found in the source file. "),
            Self::IOError(e) => write!(f, "{}", e),
        }
    }
}

impl Error for CompileError {}

impl From<std::io::Error> for CompileError {
    fn from(value: std::io::Error) -> Self {
        Self::IOError(value)
    }
}

