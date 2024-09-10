use crate::schema::*;
use diesel::{AsChangeset, Identifiable, Insertable, Queryable};
use rocket::serde::{Deserialize, Serialize};

#[derive(Queryable, Identifiable, Serialize)]
#[diesel(table_name = company_identity)]
pub struct CompanyIdentity {
    pub id: i32,
    pub name: String,
    pub taxpayer_id: String,
}

#[derive(Insertable, Deserialize)]
#[diesel(table_name = company_identity)]
pub struct NewCompanyIdentity {
    pub name: String,
    pub taxpayer_id: String,
}

#[derive(AsChangeset, Deserialize)]
#[diesel(table_name = company_identity)]
pub struct UpdateCompanyIdentity {
    pub name: Option<String>,
    pub taxpayer_id: Option<String>,
}