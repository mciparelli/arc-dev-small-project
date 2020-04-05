open Types;

[@react.component]
let make = (~data, ~onSave, ~onDelete) => {
  let (isEditing, setIsEditing) = React.useState(_state => false);
  let (isDeleteModalVisible, setDeleteModalVisibility) =
    React.useState(_state => false);
  let toggleEdit = () => setIsEditing(state => !state);
  let toggleDeleteModal = () => setDeleteModalVisibility(state => !state);
  isEditing
    ? <EditingIdea
        initialState={
          description: data->description,
          impact: data->impact,
          ease: data->ease,
          confidence: data->confidence,
        }
        onCancel={_event => toggleEdit()}
        onSubmit={newIdea => {
          toggleEdit();
          onSave(newIdea);
        }}
      />
    : <>
        {isDeleteModalVisible
           ? <div className="delete-modal text-center py-3">
               <div className="font-bigger">
                 "Are you sure?"->React.string
               </div>
               <div className="my-3">
                 "This idea will be permanently deleted."->React.string
               </div>
               <div className="d-flex justify-around mt-4">
                 <button
                   type_="button"
                   className="link text-uppercase"
                   onClick={_event => toggleDeleteModal()}>
                   "cancel"->React.string
                 </button>
                 <button
                   type_="button"
                   className="link text-uppercase text-green"
                   onClick={_event => {
                     toggleDeleteModal();
                     onDelete();
                   }}>
                   "ok"->React.string
                 </button>
               </div>
             </div>
           : React.null}
        <li className="py-2">
          <div className="idea-grid idea">
            <div> {data->description->React.string} </div>
            <div> {data->impact->string_of_int->React.string} </div>
            <div> {data->ease->string_of_int->React.string} </div>
            <div> {data->confidence->string_of_int->React.string} </div>
            <div>
              {data
               ->average
               ->Js.Float.toFixedWithPrecision(~digits=1)
               ->React.string}
            </div>
            <button
              type_="button" className="link" onClick={_event => toggleEdit()}>
              <img src={Urls.assets ++ "/pen.png"} />
            </button>
            <button
              type_="button"
              className="link"
              onClick={_event => toggleDeleteModal()}>
              <img src={Urls.assets ++ "/bin.png"} />
            </button>
          </div>
        </li>
      </>;
};