\c hotel;

-- 1. Получить список всех идентификационных записей
select
    name as "Name",
    passport_id as "Id"
from
    individual_identity
union
select
    name as "Name",
    taxpayer_id as "Id"
from
    company_identity;

-- 2. Получить количество текущих бронирований для каждого номера
select
    apartment_no as "No",
    count(*) as "Reservations"
from
    apartment_reservation
group by
    apartment_no;

-- 3. Получить список самых дешевых типов апартаментов
select
    name as "Apartment",
    cost as "Cost"
from
    apartment_type
where
    cost = (select min(cost) from apartment_type);

-- 4. Получить среднюю длительность каждого бронирования
select
    avg(reservation.check_out - reservation.check_in)
        as "Average reservation (days)"
from
    apartment_reservation reservation;

-- 5. Получить агрегированную информацию об оплаченных услугах
with
    paid_services as (
        select
            service_order.order_date,
            service_order.service_date,
            service.cost
        from
            service_order
        join
            service on service_order.service_id = service.id
        where
            paid = true
    )
select
    count(*) as "Count",
    avg(service_date - order_date) as "Average wait time (days)",
    sum(cost) as "Total cost"
from
    paid_services;

-- 6. Получись данные постояльцев-заказчиков
select
    name as "Customer",
    passport_id as "Passport ID"
from
    individual_identity
where
    id in (
        select individual_id from guest
        intersect
        select customer.individual_id from customer
    );

-- 7. Получить информацию о самых дорогих услугах
select
    service.name as "Name",
    service.description as "Description",
    service.cost as "Cost"
from
    service
where
    service.cost = (select max(cost) from service);

-- 8. Получить список постояльцев, совершивших заказ апартаментов и услуг
with
    reservation_customers as (
        select individual_id from customer
        where individual_id is not null
    ),
    service_customers as (
        select individual_id from guest
        join service_order on guest.id = service_order.guest_id
    )
select
    name as "Name"
from
    individual_identity
where
    id in (
        select individual_id from reservation_customers
        intersect
        select individual_id from service_customers
    );

-- 9. Получить транзакции всех заказчиков
with
    reservation_customers as (
        select name from individual_identity individual
        join customer on individual.id = customer.individual_id
        union
        select name from company_identity company
        join customer on company.id = customer.company_id
    ),
    service_customers as (
        select name from individual_identity individual
        where id in (
            select individual_id from guest
            join service_order on guest.id = service_order.guest_id
        )
    )
select
    name as "Name", 'Apartment reservation' as "Transaction type"
from
    reservation_customers
union
select
    name, 'Service order'
from
    service_customers;

-- 10. Получить список физических лиц, совершивших заказы
select
    name
from
    individual_identity individual
where
    exists(
        select * from customer
        join apartment_reservation on customer.id = apartment_reservation.customer_id
        where customer.individual_id = individual.id
    )
    or
    exists(
        select * from guest
        join service_order on guest.id = service_order.guest_id
        where guest.individual_id = individual.id
    );

-- 11. Получить количество постояльцев от каждой компании
select
    name as "Company",
    count(guest.id) as "Guests count"
from
    guest
join
    apartment_reservation on guest.reservation_id = apartment_reservation.id
join
    customer on apartment_reservation.customer_id = customer.id
right join
    company_identity ON customer.company_id = company_identity.id
group by
    company_identity.name;

-- 12. Вывести типы апартаментов, которые бронировались на неделю
with
    week_reservations as (
        select apartment_no from apartment_reservation
        where check_out - check_in >= 7
    ),
    week_apartments as (
        select * from apartment
        where apartment.no in (select apartment_no from week_reservations)
    )
select
    name as "Name",
    description as "Description",
    cost as "Cost"
from
    apartment_type
where
    id in (select type_id from week_apartments);

-- 13. Получить количество постояльцев в каждом номере
select
    apartment_no as "Apartment",
    count(*) as "Guests"
from
    guest
join
    apartment_reservation reservation on guest.reservation_id = reservation.id
group by
    reservation.apartment_no;

-- 14. Получить список номеров, заказанных физическими лицами
select
    no as "Apartments"
from
    apartment
where
    exists(
        select *
        from
            apartment_reservation reservation
        join
            customer on reservation.customer_id = customer.id
        where
            reservation.apartment_no = apartment.no
            and
            customer.individual_id is not null
    );

-- 15. Получить общее число дней оплаченных бронирований
select
    count(*) as "Reservations count",
    sum(check_out - check_in) as "Total days count"
from
    apartment_reservation
where
    apartment_reservation.paid = true;