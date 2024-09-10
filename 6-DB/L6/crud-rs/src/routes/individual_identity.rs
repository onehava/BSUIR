use rocket::*;
use rocket::serde::json::Json;
use rocket::serde::json::serde_json::json;
use rocket_db_pools::Connection;
use rocket_db_pools::diesel::prelude::*;

use crate::MyDatabase;
use crud_rs::ApiResponse;

use crate::schema::individual_identity;
use crate::models::individual_identity::{IndividualIdentity, NewIndividualIdentity, UpdateIndividualIdentity};

#[post("/individual_identity", format = "application/json", data = "<individual_identity>")]
pub async fn create(mut db: Connection<MyDatabase>, individual_identity: Json<NewIndividualIdentity>) -> ApiResponse {
    let response =
        diesel::insert_into(individual_identity::table)
            .values(&individual_identity.into_inner())
            .get_result::<IndividualIdentity>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/individual_identities")]
pub async fn list(mut db: Connection<MyDatabase>) -> ApiResponse {
    let response =
        individual_identity::table
            .select(individual_identity::all_columns)
            .load::<IndividualIdentity>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/individual_identity/<id>")]
pub async fn read(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response =
        individual_identity::table
            .find(id)
            .first::<IndividualIdentity>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[patch("/individual_identity/<id>", format = "application/json", data = "<individual_identity>")]
pub async fn update(mut db: Connection<MyDatabase>, individual_identity: Json<UpdateIndividualIdentity>, id: i32) -> ApiResponse {
    let response =
        diesel::update(individual_identity::table.find(id))
            .set(&individual_identity.into_inner())
            .execute(&mut db)
            .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"update": count})),
        Err(err) => ApiResponse::err(err),
    }
}

#[delete("/individual_identity/<id>")]
pub async fn delete(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response
        = diesel::delete(individual_identity::table.find(id))
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"delete": count})),
        Err(err) => ApiResponse::err(err),
    }
}