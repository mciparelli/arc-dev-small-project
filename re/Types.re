module Store = {
  [@bs.deriving {abstract: light}]
  type t = {
    jwt: option(string),
    setJwt: (option(string) => option(string)) => unit,
  };
};

type editingIdea = {
  description: string,
  impact: int,
  ease: int,
  confidence: int,
};

[@bs.deriving {abstract: light}]
type idea = {
  id: string,
  [@bs.as "average_score"]
  average: float,
  [@bs.as "content"]
  description: string,
  impact: int,
  ease: int,
  confidence: int,
  [@bs.as "created_at"]
  createdAt: int,
};

module SignupResponse = {
  [@bs.deriving {abstract: light}]
  type t = {
    [@bs.optional]
    reason: string,
    jwt: string,
    [@bs.as "refresh_token"]
    refreshToken: string,
  };
};

module User = {
  [@bs.deriving {abstract: light}]
  type t = {
    [@bs.optional]
    reason: string,
    email: string,
    name: string,
    [@bs.as "avatar_url"]
    avatarUrl: string,
  };
  type status =
    | NotCalled
    | Loading
    | Loaded(option(t));
};

module RefreshJwtResponse = {
  [@bs.deriving {abstract: light}]
  type t = {jwt: string};
};