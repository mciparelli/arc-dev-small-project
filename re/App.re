[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let store = Hooks.useStore();
  <Store.Provider value=store>
    <Layout>
      {switch (url.path) {
       | ["ideas"] => <Ideas />
       | ["signup"] => <Signup />
       | ["login"]
       | _ => <Login />
       }}
    </Layout>
  </Store.Provider>;
};