mod schema;
mod models;
mod routes;

use rocket::*;
use rocket_db_pools::Database;
use rocket_db_pools::diesel::PgPool;

#[derive(Database)]
#[database("hotel")]
pub struct MyDatabase(PgPool);

#[launch]
async fn rocket() -> _ {
    build()
        .attach(MyDatabase::init())
        .mount("/api", routes::api_routes())
}