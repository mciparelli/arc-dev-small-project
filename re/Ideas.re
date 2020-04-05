let storeContext = Store.context;
open Types;

module Header = {
  [@react.component]
  let make = () =>
    <>
      <div />
      <div className="text-capitalize text-bold"> "impact"->React.string </div>
      <div className="text-capitalize text-bold"> "ease"->React.string </div>
      <div className="text-capitalize text-bold">
        "confidence"->React.string
      </div>
      <div className="text-capitalize text-bold"> "avg."->React.string </div>
      <div />
      <div />
    </>;
};

module IdeasSet =
  Set.Make({
    type t = idea;
    let compare = (ideaA, ideaB) => String.compare(ideaA->id, ideaB->id);
  });

[@react.component]
let make = () => {
  let (ideas, setIdeas) = React.useState(() => IdeasSet.empty);
  let (isEditingIdea, setIsEditingIdea) = React.useState(() => false);
  let (currentPage, setCurrentPage) = React.useState(_ => 1);
  let (canLoadMore, setCanLoadMore) = React.useState(_ => true);
  let store = React.useContext(storeContext);
  let jwt = store->Types.Store.jwt;
  let updatePageAndLoadMoreButton =
    React.useCallback2(
      newIdeas =>
        if (newIdeas->Array.length < 10) {
          setCanLoadMore(_ => false);
        } else {
          setCurrentPage(oldPage => oldPage + 1);
        },
      (setCanLoadMore, setCurrentPage),
    );
  let getNewIdeas =
    React.useCallback3(
      jwt => {
        let%Promise newIdeas = Api.getIdeas(~jwt, ~page=currentPage);
        setIdeas(oldIdeas =>
          newIdeas->Array.to_list->IdeasSet.of_list->IdeasSet.union(oldIdeas)
        );
        updatePageAndLoadMoreButton(newIdeas);
      },
      (currentPage, updatePageAndLoadMoreButton, setIdeas),
    );
  React.useEffect2(
    () => {
      switch (jwt) {
      | None => ()
      | Some(jwt) => getNewIdeas(jwt)->ignore
      };
      None;
    },
    (jwt, setIdeas),
  );
  switch (jwt) {
  | None => React.null
  | Some(jwt) =>
    <div className="px-3 d-flex flex-column flex-fill overflow-hidden">
      <div className="px-1 py-3 d-flex align-center justify-between">
        <h2 className="text-capitalize"> "my ideas"->React.string </h2>
        <button
          type_="button"
          className="link"
          onClick={_event => setIsEditingIdea(_oldState => true)}>
          <img src={Urls.assets ++ "/btn_addanidea.png"} />
        </button>
      </div>
      <hr />
      {isEditingIdea || !ideas->IdeasSet.is_empty
         ? <>
             <ul className="px-4 overflow-auto">
               <li className="pt-2 mb-2 idea-grid position-sticky bg-white">
                 <Header />
               </li>
               {ideas
                ->IdeasSet.elements
                ->List.sort(
                    (ideaA, ideaB) =>
                      ideaB->average->int_of_float
                      - ideaA->average->int_of_float,
                    _,
                  )
                ->Array.of_list
                ->Array.map(
                    idea =>
                      <Idea
                        key={idea->id}
                        data=idea
                        onDelete={() => {
                          Api.deleteIdea(~jwt, ~id=idea->id)
                          ->Promise.then_(() => {
                              let newSet = IdeasSet.remove(idea, ideas);
                              setIdeas(_oldIdeas => newSet);
                            })
                          ->ignore
                        }}
                        onSave={newIdea => {
                          Api.updateIdea(~jwt, ~id=idea->id, ~idea=newIdea)
                          ->Promise.then_(updatedIdea => {
                              let tempSet = IdeasSet.remove(idea, ideas);
                              let newSet = IdeasSet.add(updatedIdea, tempSet);
                              setIdeas(_oldIdeas => newSet);
                            })
                          ->ignore
                        }}
                      />,
                    _,
                  )
                ->React.array}
               {isEditingIdea
                  ? <li className="py-2">
                      <EditingIdea
                        onCancel={_event => setIsEditingIdea(_state => false)}
                        onSubmit={idea =>
                          Api.createIdea(~jwt, ~idea)
                          ->Promise.then_(newIdea => {
                              setIdeas(_state => IdeasSet.add(newIdea, ideas))
                            })
                          ->ignore
                        }
                      />
                    </li>
                  : React.null}
               {canLoadMore
                  ? <li className="text-center list-style-none">
                      <button
                        type_="button"
                        className="bg-green text-white text-capitalize mx-auto my-3"
                        onClick={_event => {getNewIdeas(jwt)->ignore}}>
                        "load more"->React.string
                      </button>
                    </li>
                  : React.null}
             </ul>
           </>
         : <div className="mx-auto my-auto text-center">
             <img src={Urls.assets ++ "/bulb.png"} />
             <div className="font-big text-capitalize">
               "got ideas?"->React.string
             </div>
           </div>}
    </div>
  };
};