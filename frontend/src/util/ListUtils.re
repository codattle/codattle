let empty = list => List.length(list) == 0;

let first = list => empty(list) ? None : Some(List.hd(list));