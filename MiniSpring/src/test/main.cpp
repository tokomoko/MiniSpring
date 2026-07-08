/*#include"../core/MiniSpring.h"

#include"../web/HttpRequest.h"
#include"../web/HttpResponse.h"
#include"../web/Router.h"
#include"../web/HttpServer.h"
#include"../web/Result.h"
#include"../web/HttpError.h"
#include"../web/HttpErrors.h"
#include"../web/MimeTypeRegistry.h"

#include"../web/exceptions/HttpException.h"
#include"../web/exceptions/NotFoundException.h"

#include"../json/JsonValue.h"
#include"../json/Json.h"
#include"../json/JsonParser.h"

#include"../reflection/Field.h"
#include"../reflection/FieldBase.h"
#include"../reflection/FieldInfo.h"
#include"../reflection/TypeDescriptor.h"

#include"../resource_manager/Platform.h"
#include"../resource_manager/ResourceManager.h"
#include"../resource_manager/StaticFileHandler.h"

int main() {
	MiniSpring app;
	app.staticFiles("resources");

	app.GET("/api/test", [](HttpRequest& request) {
		HttpResponse response;
		Json json;
		json["message"] = "API works";
		response.json(json);

		return Result<HttpResponse, HttpError>::success(response);
	});

	app.listen(8080);
}*/

#include"../database/QueryResult.h"
#include"../database/DatabaseException.h"
#include"../database/sqlite/SQLiteDatabase.h"
#include"../database/migration_manager/Migration.h"
#include"../database/migration_manager/MigrationManager.h"

#include"../resource_manager/Platform.h"

#include <iostream>

void testDatabase()
{
	SQLiteDatabase db;


	try
	{
		std::cout << "Connecting...\n";

		db.connect("test.db");


		std::cout << "Connected\n";


		db.execute(
			R"(
            CREATE TABLE IF NOT EXISTS users(
                id INTEGER PRIMARY KEY,
                name TEXT,
                age INTEGER,
                active INTEGER
            )
            )"
		);


		std::cout << "Table created\n";


		auto statement =
			db.prepare(
				"INSERT INTO users(id,name,age,active) VALUES(?,?,?,?)"
			);


		statement->bind(0, 1);
		statement->bind(1, "Tornike");
		statement->bind(2, 22);
		statement->bind(3, true);


		statement->executeUpdate();


		std::cout << "Inserted\n";



		auto result =
			db.execute(
				"SELECT * FROM users"
			);


		std::cout
			<< "Rows: "
			<< result->rowCount()
			<< "\n";



		Value id =
			result->get(0, "id");

		Value name =
			result->get(0, "name");

		Value age =
			result->get(0, "age");

		Value active =
			result->get(0, "active");



		std::cout
			<< "ID: "
			<< id.get<int>()
			<< "\n";


		std::cout
			<< "Name: "
			<< name.get<std::string>()
			<< "\n";


		std::cout
			<< "Age: "
			<< age.get<int>()
			<< "\n";


		std::cout
			<< "Active: "
			<< active.get<int>()
			<< "\n";

	}
	catch (const std::exception& e)
	{
		std::cout
			<< "Database error: "
			<< e.what()
			<< "\n";
	}
}

int main() {
	try {
		SQLiteDatabase db;
		db.connect("test.db");
		MigrationManager manager(db, executableDirectory()/"resources/migrations");
		manager.migrate();

	} catch (const DatabaseException& e) {
		std::cerr << e.what() << std::endl;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}