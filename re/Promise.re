include Js.Promise;
let let_ = (promise, callback) =>
  then_(res => res->callback->resolve, promise);

let then_ = let_;