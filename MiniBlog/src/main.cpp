#include<MiniSpring.h>

#include"models/Post.h"
#include"models/PostCreateRequest.h"
#include"models/PostUpdateRequest.h"

#include"controllers/PostController.h"

#include<iostream>

int main() {
	try {
		MiniSpringApp app;
		app.server()
			.port(8080);

		app.database()
			.file("blog.db")
			.migrations("resources/migrations");

		app.staticFiles()
			.directory("resources/static");

		app.entities()
			.registerEntity<Post>()
			.registerEntity<PostCreateRequest>()
			.registerEntity<PostUpdateRequest>();

		app.build();

		app.controllers()
			.registerController<PostController>(app);
		
		app.initControllers();
		app.migrate();
		app.listen();
	}
	catch (const DatabaseException& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const HttpException& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}