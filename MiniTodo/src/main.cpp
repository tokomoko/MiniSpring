#include<MiniSpring.h>

#include "models/TodoEntity.h"
#include "controllers/TodoController.h"

#include<iostream>

int main() {
	try {
		MiniSpringApp app;
		app.database("todo.db");
		app.migrations("resources/migrations");
		app.staticFiles("resources/static");

		registerTodoEntity(app.entities());

		app.migrate();
		app.registerController<TodoController>(app);
		app.listen(8080);
	}
	catch (DatabaseException e) {
		std::cerr << e.what() << std::endl;
		while (1);
	}
}