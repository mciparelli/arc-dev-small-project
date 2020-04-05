[@react.component]
let make = (~user=?) => {
  let store = React.useContext(Store.context);
  <aside className="bg-green py-3 text-center px-5">
    <img src={Urls.assets ++ "/IdeaPool_icon.png"} />
    <div className="text-capitalize text-white mt-1">
      "The Idea Pool"->React.string
    </div>
    {switch (user) {
     | None => React.null
     | Some(user) =>
       <>
         <hr className="my-3 border-white" />
         <img
           className="border-radius-50 avatar"
           src={user->Types.User.avatarUrl}
         />
         <p className="text-white font-big">
           {user->Types.User.name->React.string}
         </p>
         <button
           type_="button"
           className="link logout"
           onClick={_event =>
             Api.logout(store->Types.Store.jwt)
             ->Promise.then_(() => {
                 store->Types.Store.setJwt(_oldJwt => None)
               })
             ->ignore
           }>
           "Log out"->React.string
         </button>
       </>
     }}
  </aside>;
};