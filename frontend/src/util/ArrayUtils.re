let mapToList = (map: 'a => 'b, array: array('a)): list('b) => array |> Array.map(map) |> Array.to_list;

let mapiToList = (map: (int, 'a) => 'b, array: array('a)): list('b) => array |> Array.mapi(map) |> Array.to_list;