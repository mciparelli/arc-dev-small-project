[@bs.val] [@bs.scope "localStorage"]
external set: (string, 'a) => unit = "setItem";

[@bs.val] [@bs.return nullable] [@bs.scope "localStorage"]
external get: string => option('a) = "getItem";

[@bs.val] [@bs.scope "localStorage"]
external remove: string => unit = "removeItem";