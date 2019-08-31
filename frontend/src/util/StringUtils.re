open Rationale.Function;
open String;

let isEmpty: string => bool = string => length(string) === 0;

let isBlank: string => bool = trim ||> isEmpty;

let toNonBlank: string => option(string) = string => isBlank(string) ? None : Some(string);