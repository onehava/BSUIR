create database hotel;
\c hotel;

create table apartment_type
(
    id          serial primary key,
    name        character varying(64) not null,
    description text,
    capacity    integer               not null check (capacity > 0),
    cost        integer               not null check (cost > 0),

    constraint apartment_type_name_and_capacity_unique unique (name, capacity)
);

create table service
(
    id          serial primary key,
    name        character varying(64) not null unique,
    description text,
    cost        integer               not null check (cost > 0)
);

create table apartment
(
    no      integer primary key check (no > 0),
    free    boolean                                not null,
    type_id integer references apartment_type (id) not null
);

create table individual_identity
(
    id          serial primary key,
    name        character varying(64) not null,
    passport_id character varying(14) not null unique
);

create table company_identity
(
    id          serial primary key,
    name        character varying(64) not null,
    taxpayer_id character varying(9)  not null unique
);

create table customer
(
    id            serial primary key,
    individual_id integer references individual_identity (id) unique,
    company_id    integer references company_identity (id) unique,

    constraint customer_id_type check (
        individual_id is null and company_id is not null
            or
        individual_id is not null and company_id is null)
);

create table apartment_reservation
(
    id           serial primary key,
    date         date                                               not null,
    paid         boolean                                            not null,
    check_in     date                                               not null,
    check_out    date                                               not null,
    apartment_no integer references apartment (no)                  not null,
    customer_id  integer references customer (id) on delete cascade not null,

    constraint apartment_reservation_consequence_dates check (date <= check_in and check_in < check_out)
);

create table guest
(
    id             serial primary key,
    individual_id  integer references individual_identity (id)                     not null unique,
    reservation_id integer references apartment_reservation (id) on delete cascade not null
);

create table service_order
(
    guest_id     integer references guest (id) on delete cascade not null,
    service_id   integer references service (id)                 not null,

    order_date   date                                            not null,
    service_date date                                            not null,
    paid         boolean                                         not null,

    constraint guest_service_pk primary key (guest_id, service_id),
    constraint service_order_consequence_dates check (order_date <= service_date)
);