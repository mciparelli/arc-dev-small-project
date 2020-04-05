open Types;
open Utils;

type state = editingIdea;

type action =
  | SetDescription(string)
  | SetImpact(int)
  | SetEase(int)
  | SetConfidence(int)
  | Reset;

let defaultState = {description: "", impact: 10, ease: 10, confidence: 10};

let reducer = (state, action) =>
  switch (action) {
  | SetDescription(description) => {...state, description}
  | SetImpact(impact) => {...state, impact}
  | SetEase(ease) => {...state, ease}
  | SetConfidence(confidence) => {...state, confidence}
  | Reset => defaultState
  };

[@react.component]
let make = (~onCancel, ~onSubmit, ~initialState=defaultState) => {
  let (state, send) = React.useReducer(reducer, initialState);
  let {description, impact, ease, confidence} = state;
  let average = (impact + ease + confidence) / 3;
  let impactWithinBounds = impact > 0 && impact < 11;
  let easeWithinBounds = ease > 0 && ease < 11;
  let confidenceWithinBounds = confidence > 0 && confidence < 11;
  let canSave =
    impactWithinBounds
    && easeWithinBounds
    && confidenceWithinBounds
    && description !== "";
  <form
    className="d-flex align-center idea-grid"
    onSubmit={event => {
      event->ReactEvent.Form.preventDefault;
      Reset->send;
      onSubmit(state);
    }}>
    <input
      type_="text"
      required=true
      autoFocus=true
      value=description
      onChange={event => event->getInputValue->SetDescription->send}
    />
    <input
      type_="number"
      name="impact"
      min=1
      max="10"
      value={impact->string_of_int}
      onChange={event => event->getInputValue->int_of_string->SetImpact->send}
    />
    <input
      type_="number"
      name="ease"
      min=1
      max="10"
      value={ease->string_of_int}
      onChange={event => event->getInputValue->int_of_string->SetEase->send}
    />
    <input
      type_="number"
      name="confidence"
      min=1
      max="10"
      value={confidence->string_of_int}
      onChange={event =>
        event->getInputValue->int_of_string->SetConfidence->send
      }
    />
    <div> {average->string_of_int->React.string} </div>
    <button type_="submit" className="link" disabled={!canSave}>
      <img src={Urls.assets ++ "/Confirm_V.png"} />
    </button>
    <button type_="button" className="link" onClick=onCancel>
      <img src={Urls.assets ++ "/Cancel_X.png"} />
    </button>
  </form>;
};