use rocket::*;
use rocket::serde::json::Json;
use rocket::serde::json::serde_json::json;
use rocket_db_pools::Connection;
use rocket_db_pools::diesel::prelude::*;

use crate::MyDatabase;
use crud_rs::ApiResponse;

use crate::schema::company_identity;
use crate::models::company_identity::{CompanyIdentity, NewCompanyIdentity, UpdateCompanyIdentity};

#[post("/company_identity", format = "application/json", data = "<company_identity>")]
pub async fn create(mut db: Connection<MyDatabase>, company_identity: Json<NewCompanyIdentity>) -> ApiResponse {
    let response =
        diesel::insert_into(company_identity::table)
            .values(&company_identity.into_inner())
            .get_result::<CompanyIdentity>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/company_identities")]
pub async fn list(mut db: Connection<MyDatabase>) -> ApiResponse {
    let response =
        company_identity::table
            .select(company_identity::all_columns)
            .load::<CompanyIdentity>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/company_identity/<id>")]
pub async fn read(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response =
        company_identity::table
            .find(id)
            .first::<CompanyIdentity>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[patch("/company_identity/<id>", format = "application/json", data = "<company_identity>")]
pub async fn update(mut db: Connection<MyDatabase>, company_identity: Json<UpdateCompanyIdentity>, id: i32) -> ApiResponse {
    let response =
        diesel::update(company_identity::table.find(id))
            .set(&company_identity.into_inner())
            .execute(&mut db)
            .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"update": count})),
        Err(err) => ApiResponse::err(err),
    }
}

#[delete("/company_identity/<id>")]
pub async fn delete(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response
        = diesel::delete(company_identity::table.find(id))
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"delete": count})),
        Err(err) => ApiResponse::err(err),
    }
}