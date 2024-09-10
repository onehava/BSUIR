use rocket::http::{ContentType, Status};
use rocket::{Request, Response, response};
use rocket::response::Responder;
use rocket::serde::json::{json, serde_json, Value};
use serde::Serialize;

#[derive(Debug)]
pub struct ApiResponse {
    pub json: Value,
    pub status: Status,
}

impl ApiResponse {
    pub fn from(r: Result<impl Serialize, diesel::result::Error>) -> Self {
        match r {
            Ok(v) => Self::ok(v),
            Err(v) => Self::err(v),
        }
    }

    pub fn ok(v: impl Serialize) -> Self {
        ApiResponse {
            json: serde_json::to_value(v).unwrap(),
            status: Status::Ok,
        }
    }

    pub fn err(v: diesel::result::Error) -> Self {
        ApiResponse {
            json: json!({"error": v.to_string()}),
            status: Status::BadRequest,
        }
    }
}

impl<'r> Responder<'r, 'r> for ApiResponse {
    fn respond_to(self, req: &Request) -> response::Result<'r> {
        Response::build_from(self.json.respond_to(req).unwrap())
            .status(self.status)
            .header(ContentType::JSON)
            .ok()
    }
}