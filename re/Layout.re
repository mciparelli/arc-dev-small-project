open Types;

[@react.component]
let make = (~children) => {
  let status = Hooks.useCurrentUserStatus();
  React.useEffect1(
    () => {
      switch (status) {
      | Loaded(user) =>
        switch (user) {
        | Some(_user) => ReasonReactRouter.push("/ideas")
        | None => ReasonReactRouter.push("/login")
        }
      | _ => ()
      };
      None;
    },
    [|status|],
  );

  <>
    <Sidebar
      user=?{
        switch (status) {
        | Loaded(user) => user
        | _ => None
        }
      }
    />
    <main
      className={
        "flex-fill text-black d-flex flex-column"
        ++ (status === User.Loading ? " loading" : "")
      }>
      children
    </main>
  </>;
};