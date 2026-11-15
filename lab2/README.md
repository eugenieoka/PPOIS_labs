# Лабораторная работа №2 — Транспортная компания

Домен: **транспортная компания**. Реализована объектная модель перевозок: сотрудники,
водители, автопарк, грузы, склад, заказы, доставка, финансы, аналитика и клиентский
сервис.

Работа оформлена как отдельный проект `lab2/` в общем репозитории лабораторных работ.
Код домена не зависит от консольного интерфейса: CLI использует только публичные
заголовки `include/transport/...`.

## Состав проекта

```text
lab2/
├── CMakeLists.txt
├── .clang-format
├── README.md
├── include/transport/       публичные заголовки домена
│   ├── core/
│   ├── staff/
│   ├── fleet/
│   ├── cargo/
│   ├── warehouse/
│   ├── orders/
│   ├── finance/
│   ├── analytics/
│   ├── service/
│   └── exceptions/
├── src/                     реализация домена
├── cli/                     консольное меню
└── tests/                   GoogleTest-тесты
```

Принципы реализации:

- один доменный класс — один `.h` и один `.cpp`;
- владение сущностями — `std::shared_ptr` (компания хранит сотрудников, транспорт,
  клиентов и заказы; заказ ссылается на клиента и груз; доставка — на заказ, водителя
  и транспорт);
- проверки инвариантов выполняются в конструкторах и публичных методах;
- ошибки предметной области выражаются отдельными исключениями;
- константы вынесены в `transport::constants`, перечисления — в `transport/Enums.h`;
- строки статусов выводятся через `toString(...)`;
- геттеры не считаются «содержательным поведением» и в статистике методов не учтены.

## Сборка, тесты и запуск

Из корня репозитория:

```bash
cmake -S lab2 -B build/lab2 -DCMAKE_BUILD_TYPE=Debug
cmake --build build/lab2 --parallel
ctest --test-dir build/lab2 --output-on-failure
./build/lab2/transport_cli
```

Release-сборка только консольного приложения:

```bash
cmake -S lab2 -B build/lab2-release -DCMAKE_BUILD_TYPE=Release
cmake --build build/lab2-release --target transport_cli
./build/lab2-release/transport_cli
```

Проверка покрытия и статический анализ:

```bash
gcovr -r . --filter 'lab2/src/' --exclude-unreachable-branches \
  --fail-under-line 90 --print-summary build/lab2

cppcheck --enable=warning,performance,portability --error-exitcode=1 \
  --std=c++17 --suppress=missingIncludeSystem -I lab2/include lab2/src lab2/cli
```

## Метрики

| Показатель | Значение |
|---|---:|
| Доменные классы | 53 |
| Классы исключений | 17 |
| Поля доменных классов | 194 |
| Содержательные поведения (без геттеров и конструкторов) | 229 |
| Ассоциации и связи | 84 |
| Тесты GoogleTest | 105 |
| Покрытие строк | 99.2% |
| Покрытие функций | 100% |

## Консольное меню

`transport_cli` создаёт демонстрационную компанию с клиентом, водителем, грузовиком,
заказом и складом. Меню позволяет:

1. показать сводку компании;
2. назначить водителя и транспорт на заказ;
3. вывести историю отслеживания;
4. принять и выдать груз на складе;
5. сформировать финансовый отчёт;
6. обработать обращение клиента и показать бонусный уровень;
7. вывести аналитическую сводку.

CLI не содержит бизнес-правил: все проверки, расчёты и переходы состояний выполняет
доменный слой.

## Доменные классы

В таблицах перечислены поля и ключевые поведения. Тривиальные getter-методы опущены.

### core — базовые типы

| Класс | Поля | Поведения | Связи |
|---|---|---|---|
| `Address` | `street`, `city`, `postalCode`, `building` | `format`, `sameCity` | значение в `Employee`, `Client`, `Warehouse`, `RoutePoint` |
| `GPSPosition` | `latitude`, `longitude` | `isValid`, `distanceTo`, `approxEquals` | значение в `RoutePoint` |
| `Money` | `amountInCents`, `currency` | `add`, `subtract`, `multiply`, `isNegative`, `isZero`, `format`, `operator==` | значение в `Salary`, `Client`, `Invoice`, `Payment`, `Cargo`, `Insurance`, `MaintenanceRecord` |
| `TransportDate` | `day`, `month`, `year` | `isValid`, `isBefore`, `isAfter`, `sameDay`, `addDays`, `daysUntil`, `toString` | значение в `Employee`, `DrivingLicense`, `Order`, `Delivery`, `Invoice`, `Payment` |

### staff — сотрудники

| Класс | Поля | Поведения | Связи |
|---|---|---|---|
| `Salary` | `base`, `bonus`, `deductions` | `net`, `addBonus`, `addDeduction`, `isPositive` | три `Money` |
| `DrivingLicense` | `number`, `category`, `issuedOn`, `expiresOn` | `isValidOn`, `coversCategory`, `renew` | две `TransportDate` |
| `Employee` | `employeeId`, `fullName`, `address`, `phone`, `salary`, `hireDate`, `active` | `annualCost`, `tenureYears`, `giveRaise`, `activate`, `deactivate`, `roleName` | `Address`, `Salary`, `TransportDate` |
| `Driver` | `license`, `experienceYears`, `completedDeliveries`, `rating`, `available` | `canDrive`, `assign`, `completeDelivery`, `updateRating`, `roleName` | наследует `Employee`, содержит `DrivingLicense` |
| `Dispatcher` | `region`, `activeOrders`, `maxOrders`, `shift` | `canHandleMore`, `assignOrder`, `releaseOrder`, `closeShift`, `roleName` | наследует `Employee` |
| `Manager` | `department`, `budget` | `remainingBudget`, `approveExpense`, `increaseBudget`, `roleName` | наследует `Employee`, содержит `Money` |
| `HRManager` | `hiredCount`, `openPositions` | `hire`, `fire`, `openPosition`, `closePosition`, `roleName` | наследует `Manager` |
| `WarehouseManager` | `warehouse` | `receiveCargo`, `releaseCargo`, `inventoryValue`, `capacityReport`, `roleName` | наследует `Manager`, ссылается на `Warehouse`, `StorageUnit`, `Cargo` |
| `Accountant` | `processedInvoices`, `taxCalculator` | `issueInvoice`, `processPayment`, `calculateSalary`, `taxReport`, `roleName` | наследует `Employee`, использует `Order`, `Client`, `Invoice`, `Payment`, `BankAccount`, `TaxCalculator` |
| `Mechanic` | `specialization`, `repairsCompleted` | `canRepair`, `performMaintenance`, `completeRepair`, `roleName` | наследует `Employee`, использует `Vehicle`, `MaintenanceRecord` |

### fleet — автопарк

| Класс | Поля | Поведения | Связи |
|---|---|---|---|
| `Machine` | `serialNumber`, `fuelType`, `running`, `fuelPerHour` | `start`, `stop`, `consumeFuel` | значение в `Vehicle` |
| `Vehicle` | `plateNumber`, `capacityKg`, `capacityM3`, `mileageKm`, `status`, `baseRatePerKm`, `machine` | `canCarry`, `addMileage`, `changeStatus`, `needsMaintenance`, `isAvailable`, `baseCost`, `typeName` | содержит `Machine`, проверяет `Cargo` |
| `Truck` | `axles`, `trailer` | `maxPayload`, `attachTrailer`, `detachTrailer`, `typeName` | наследует `Vehicle`, содержит `Trailer` |
| `Van` | `refrigerated`, `slidingDoors` | `canCarryPerishable`, `loadingCost`, `typeName` | наследует `Vehicle`, проверяет `PerishableCargo` |
| `Trailer` | `capacityKg`, `capacityM3`, `attached` | `attach`, `detach`, `isAvailable`, `fitsCargo` | проверяет `Cargo` |
| `MaintenanceRecord` | `vehiclePlate`, `mechanicId`, `performedOn`, `cost`, `description` | `totalCostWithTax`, `isRecent`, `summary` | `Money`, `TransportDate` |
| `Insurance` | `policyNumber`, `vehiclePlate`, `premium`, `validFrom`, `validTo` | `isActiveOn`, `renew`, `annualCost` | `Money`, две `TransportDate` |

### cargo — грузы

| Класс | Поля | Поведения | Связи |
|---|---|---|---|
| `Cargo` | `cargoId`, `description`, `weightKg`, `volumeM3`, `declaredValue` | `density`, `isOverweight`, `handlingCost`, `typeName`, `summary` | `Money` |
| `FragileCargo` | `packaging`, `maxDropHeightCm` | `handlingCost`, `requiresSpecialPackaging`, `typeName` | наследует `Cargo` |
| `PerishableCargo` | `expiryDate`, `minTempC`, `maxTempC` | `isExpiredOn`, `requiresRefrigeration`, `remainingDays`, `handlingCost`, `typeName` | наследует `Cargo`, содержит `TransportDate` |

### warehouse — склад

| Класс | Поля | Поведения | Связи |
|---|---|---|---|
| `Rack` | `number`, `levels`, `capacityKg`, `usedCapacityKg`, `cargoIds` | `freeCapacity`, `place`, `take`, `isEmpty` | хранит идентификаторы `Cargo`, принимает `Cargo` |
| `StorageUnit` | `unitId`, `capacityKg`, `zoneName`, `usedCapacityKg`, `cargoIds` | `canStore`, `store`, `release`, `occupancy` | принимает `Cargo` |
| `StorageZone` | `name`, `temperatureC`, `racks` | `addRack`, `findFreeRack`, `totalCapacity`, `occupancy`, `accepts` | содержит `Rack`, проверяет `PerishableCargo` |
| `Warehouse` | `name`, `address`, `zones`, `inventoryValue` | `addZone`, `receiveCargo`, `shipCargo`, `totalCapacity`, `occupancy`, `inventoryValue` | `Address`, `StorageZone`, `StorageUnit`, `Cargo`, `Money` |

### orders — заказы и доставка

| Класс | Поля | Поведения | Связи |
|---|---|---|---|
| `Client` | `clientId`, `fullName`, `address`, `phone`, `balance`, `loyaltyPoints`, `blacklisted` | `deposit`, `withdraw`, `canPay`, `addLoyaltyPoints`, `spendLoyaltyPoints`, `discountRate`, `block`, `unblock` | `Address`, `Money` |
| `RoutePoint` | `address`, `position`, `sequence`, `orderId` | `sameAs`, `distanceTo`, `describe` | `Address`, `GPSPosition` |
| `Route` | `name`, `points`, `distanceKm` | `addPoint`, `totalDistance`, `estimatedDuration`, `contains`, `optimize` | содержит `RoutePoint` |
| `Order` | `orderId`, `client`, `cargo`, `route`, `price`, `status`, `createdOn`, `deadline`, `driverId`, `vehiclePlate` | `calculatePrice`, `assignDriver`, `assignVehicle`, `changeStatus`, `cancel`, `isDeliverable`, `isOverdue` | `Client`, `Cargo`, `Route`, `Driver`, `Vehicle`, `Money`, `TransportDate` |
| `ExpressDelivery` | `guaranteedHours`, `surchargeRate` | `calculatePrice`, `isExpress`, `estimatedDeliveryDate` | наследует `Order` |
| `Delivery` | `deliveryId`, `order`, `driver`, `vehicle`, `status`, `startedAt`, `completedAt` | `start`, `complete`, `fail`, `durationHours`, `isOnTime` | `Order`, `Driver`, `Vehicle`, `TransportDate` |
| `TrackingSystem` | `events` | `addEvent`, `historyFor`, `currentStatus`, `lastEvent`, `hasHistory`, `clear` | события по идентификатору `Order`, `TransportDate` |
| `LogisticsManager` | `deliveries`, `dispatchers` | `addDispatcher`, `assignDelivery`, `completeDelivery`, `findDelivery`, `dailyReport` | содержит `Delivery`, ссылается на `Dispatcher`, `Order`, `Driver`, `Vehicle` |

### finance — финансы

| Класс | Поля | Поведения | Связи |
|---|---|---|---|
| `BankAccount` | `iban`, `balance` | `deposit`, `withdraw`, `transferTo`, `canWithdraw` | `Money` |
| `Invoice` | `number`, `orderId`, `clientId`, `amount`, `issuedOn`, `dueOn`, `paid` | `markPaid`, `remaining`, `isOverdue`, `totalWithVat`, `pay` | `Money`, две `TransportDate`, идентификаторы `Order`/`Client` |
| `Payment` | `invoiceNumber`, `amount`, `method`, `status`, `paidAt` | `process`, `refund`, `isSuccessful` | `Money`, `TransportDate`, ссылка на `Invoice` по номеру |
| `ExpenseTracker` | `expenses` | `addExpense`, `total`, `byCategory`, `largest` | пары «категория — `Money`» |
| `ProfitAnalyzer` | `income`, `expenses` | `recordIncome`, `addExpense`, `grossProfit`, `margin`, `monthlyProfit`, `isProfitable` | `Money`, содержит `ExpenseTracker` |
| `TaxCalculator` | `vatRate`, `incomeTaxRate` | `vat`, `incomeTax`, `netIncome` | `Money` |
| `CompanyFinance` | `account`, `invoices`, `payments`, `expenseTracker`, `profitAnalyzer`, `taxCalculator` | `addInvoice`, `recordIncome`, `recordExpense`, `balance`, `unpaidInvoices`, `monthlyReport`, `taxReport` | `BankAccount`, `Invoice`, `Payment`, `ExpenseTracker`, `ProfitAnalyzer`, `TaxCalculator` |

### analytics — аналитика

| Класс | Поля | Поведения | Связи |
|---|---|---|---|
| `PerformanceMetric` | `name`, `value`, `target` | `isOnTarget`, `deviation`, `update` | — |
| `FuelConsumption` | `distanceKm`, `litersUsed`, `pricePerLiter` | `per100Km`, `totalCost`, `isEfficient` | `Money` |
| `TrafficInfo` | `region`, `delayMinutes`, `congestionLevel` | `delayFactor`, `adjustedDuration`, `isSevere` | — |
| `Metrics` | `performanceMetrics`, `fuel`, `traffic` | `addMetric`, `averageValue`, `summarize` | `PerformanceMetric`, `FuelConsumption`, `TrafficInfo` |
| `CustomerFeedback` | `clientId`, `orderId`, `rating`, `comment`, `submittedOn` | `isPositive`, `summary` | идентификаторы `Client`/`Order`, `TransportDate` |
| `ServiceQualityMonitor` | `feedbacks` | `addFeedback`, `averageRating`, `complaintRate`, `topComplaints` | содержит `CustomerFeedback` |

### service — сервис и фасад компании

| Класс | Поля | Поведения | Связи |
|---|---|---|---|
| `LoyaltyProgram` | `pointsPerCurrencyUnit`, `silverThreshold`, `goldThreshold` | `earn`, `canRedeem`, `redeem`, `tierFor` | `Client`, `Money` |
| `CustomerService` | `clients`, `monitor` | `registerClient`, `fileComplaint`, `findClient` | `Client`, `CustomerFeedback`, `ServiceQualityMonitor` |
| `ServiceManagement` | `loyalty`, `customerService`, `monitor` | `onboardClient`, `processComplaint`, `loyaltyReport` | `LoyaltyProgram`, `CustomerService`, `ServiceQualityMonitor`, `Client` |
| `TransportCompany` | `name`, `address`, `employees`, `fleet`, `clients`, `orders`, `warehouses`, `finance`, `tracking`, `customerService` | `hire`, `addClient`, `addVehicle`, `addWarehouse`, `placeOrder`, `dispatchOrder`, `cancelOrder`, `findOrder`, `findClient`, `financialReport`, `qualityReport` | агрегирует `Employee`, `Vehicle`, `Client`, `Order`, `Warehouse`, `CompanyFinance`, `TrackingSystem`, `CustomerService` |

## Ассоциации и связи

Основные связи модели:

- наследование: `Driver`, `Dispatcher`, `Manager`, `Accountant`, `Mechanic` → `Employee`;
  `HRManager` → `Manager`; `Truck`, `Van` → `Vehicle`; `FragileCargo`, `PerishableCargo`
  → `Cargo`; `ExpressDelivery` → `Order`;
- композиция/агрегация: `TransportCompany` → сотрудники, транспорт, клиенты, заказы,
  склады, финансы, отслеживание, клиентский сервис;
- `Warehouse` → `StorageZone` → `Rack`; `Warehouse` принимает `StorageUnit` и `Cargo`;
- `Order` → `Client`, `Cargo`, `Route`, `Driver` (по идентификатору), `Vehicle`
  (по номеру), `Money`, `TransportDate`;
- `Delivery` → `Order`, `Driver`, `Vehicle`, даты начала и завершения;
- `CompanyFinance` → `BankAccount`, `Invoice`, `Payment`, `ExpenseTracker`,
  `ProfitAnalyzer`, `TaxCalculator`;
- `ServiceManagement` → `LoyaltyProgram`, `CustomerService`, `ServiceQualityMonitor`;
- `CustomerService` → `Client`, `CustomerFeedback`, `ServiceQualityMonitor`.

Всего в модели 84 ассоциации, включая наследование, композицию, агрегацию и
параметрические зависимости.

## Исключения

Все исключения находятся в `include/transport/exceptions/` и наследуются от
`TransportException`, который, в свою очередь, наследуется от `std::runtime_error`.

| Исключение | Когда возникает |
|---|---|
| `TransportException` | базовое исключение домена; общие нарушения инвариантов |
| `InvalidLocationException` | пустые поля адреса, координаты вне допустимого диапазона |
| `InvalidDateException` | несуществующая календарная дата |
| `InvalidMoneyException` | нечисловая сумма, неверный код валюты, несовпадение валют, отрицательные компоненты |
| `InvalidClientDataException` | некорректный идентификатор/имя/телефон клиента, отрицательные бонусы |
| `InvalidCargoDataException` | некорректные масса, объём, стоимость, упаковка, температурный диапазон |
| `CargoOverweightException` | груз превышает допустимую массу (резерв для специализированных проверок) |
| `VehicleOverloadException` | транспорт не может перевезти груз по массе или объёму |
| `VehicleNotAvailableException` | транспорт занят, на обслуживании или выведен из эксплуатации |
| `DriverNotAvailableException` | водитель уже назначен на другой рейс |
| `InvalidLicenseException` | пустой номер, неизвестная категория, некорректные даты водительского удостоверения |
| `OrderNotFoundException` | заказ, доставка или клиент не найдены |
| `InvalidOrderStateException` | недопустимый переход статуса заказа/доставки, повторное завершение |
| `InsufficientFundsException` | недостаточно средств на счёте клиента, компании или бюджете менеджера |
| `PaymentException` | повторная оплата, возврат невыполненного платежа, неверная ставка НДС |
| `WarehouseCapacityException` | превышена вместимость стеллажа или складской единицы |
| `InvalidRatingException` | рейтинг водителя или отзыв вне диапазона 0–5 / 1–5 |

## Тестирование

Тесты разделены по модулям:

- `CoreTest.cpp` — `Address`, `GPSPosition`, `Money`, `TransportDate`, перечисления;
- `StaffTest.cpp` — зарплата, лицензии, сотрудники и роли;
- `FleetTest.cpp` — машины, транспорт, прицепы, обслуживание, страховка;
- `CargoWarehouseTest.cpp` — грузы и складские единицы;
- `OrdersTest.cpp` — клиенты, маршруты, заказы, доставка, отслеживание, логистика;
- `FinanceTest.cpp` — счета, платежи, расходы, прибыль, налоги, финансы компании;
- `AnalyticsTest.cpp` — метрики, расход топлива, пробки, отзывы, качество сервиса;
- `ServiceTest.cpp` — лояльность, клиентский сервис и фасад компании.

Тесты запускаются без IDE:

```bash
ctest --test-dir build/lab2 --output-on-failure
```

Текущий результат: **105 тестов, 99.2% покрытия строк, 100% покрытия функций**.

## CI и релиз

Workflow `.github/workflows/ci.yml`:

1. собирает `lab2` в Debug;
2. запускает все тесты;
3. проверяет покрытие строк не ниже 90% через `gcovr`;
4. запускает `cppcheck`;
5. собирает Release-версию `transport_cli`;
6. упаковывает `transport_cli-linux-x86_64.tar.gz`;
7. публикует архив в GitHub Releases сразу после успешного push в `main` или
   `feature/lab2-*`.

Релиз создаётся автоматически, без мержа и без ручного тега. CI использует тег
`lab2-build-<номер запуска>` и прикрепляет к нему готовый к запуску бинарник.
Релизы из feature-веток помечаются как prerelease, релизы из `main` — как обычные.
Каждый следующий push в отслеживаемую ветку создаёт новый релиз.
