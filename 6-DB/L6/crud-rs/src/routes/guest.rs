use rocket::*;
use rocket::serde::json::Json;
use rocket::serde::json::serde_json::json;
use rocket_db_pools::Connection;
use rocket_db_pools::diesel::prelude::*;

use crate::MyDatabase;
use crud_rs::ApiResponse;

use crate::schema::guest;
use crate::models::guest::{Guest, NewGuest, UpdateGuest};

#[post("/guest", format = "application/json", data = "<guest>")]
pub async fn create(mut db: Connection<MyDatabase>, guest: Json<NewGuest>) -> ApiResponse {
    let response =
        diesel::insert_into(guest::table)
            .values(&guest.into_inner())
            .get_result::<Guest>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/guests")]
pub async fn list(mut db: Connection<MyDatabase>) -> ApiResponse {
    let response =
        guest::table
            .select(guest::all_columns)
            .load::<Guest>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[get("/guest/<id>")]
pub async fn read(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response =
        guest::table
            .find(id)
            .first::<Guest>(&mut db)
            .await;

    ApiResponse::from(response)
}

#[patch("/guest/<id>", format = "application/json", data = "<guest>")]
pub async fn update(mut db: Connection<MyDatabase>, guest: Json<UpdateGuest>, id: i32) -> ApiResponse {
    let response =
        diesel::update(guest::table.find(id))
            .set(&guest.into_inner())
            .execute(&mut db)
            .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"update": count})),
        Err(err) => ApiResponse::err(err),
    }
}

#[delete("/guest/<id>")]
pub async fn delete(mut db: Connection<MyDatabase>, id: i32) -> ApiResponse {
    let response
        = diesel::delete(guest::table.find(id))
        .execute(&mut db)
        .await;

    match response {
        Ok(count) => ApiResponse::ok(json!({"delete": count})),
        Err(err) => ApiResponse::err(err),
    }
}
