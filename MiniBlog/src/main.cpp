#include<MiniSpring.h>

#include"models/Post.h"
#include"models/PostCreateRequest.h"
#include"models/PostUpdateRequest.h"

#include"controllers/PostController.h"

#include<iostream>

int main() {
	try {
		MiniSpringApp app;
		app.database("blog.db");
		app.migrations("resources/migrations");
		app.staticFiles("resources/static");

		app.registerEntity<Post>();
		app.registerEntity<PostCreateRequest>();
		app.registerEntity<PostUpdateRequest>();

		app.migrate();
		app.registerController<PostController>(app);
		app.listen(8080);
	}
	catch (DatabaseException e) {
		std::cerr << e.what() << std::endl;
	}
	catch (HttpException e) {
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}
}