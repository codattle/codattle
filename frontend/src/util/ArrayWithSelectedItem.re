type t('a) = {
  array: array('a),
  selectedIndex: int,
};

let fromArray = array => {array, selectedIndex: 0};

let getSelected = t =>
  if (Array.length(t.array) == 0) {
    None;
  } else {
    Some(t.array[t.selectedIndex]);
  };

let canNext = t => t.selectedIndex < Array.length(t.array) - 1;

let canPrevious = t => t.selectedIndex > 0;

let next = t => {array: t.array, selectedIndex: canNext(t) ? t.selectedIndex + 1 : t.selectedIndex};

let previous = t => {array: t.array, selectedIndex: canPrevious(t) ? t.selectedIndex - 1 : t.selectedIndex};