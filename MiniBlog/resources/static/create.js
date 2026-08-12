async function createPost()
{

    const title =
        document.getElementById("title")
        .value;


    const summary =
        document.getElementById("summary")
        .value;


    const content =
        document.getElementById("content")
        .value;



    if(title.trim() === "")
    {
        alert("Title required");
        return;
    }



    const response =
        await fetch(
            "/posts",
            {
                method:"POST",

                headers:
                {
                    "Content-Type":
                        "application/json"
                },


                body:
                    JSON.stringify(
                    {
                        title:title,

                        summary:summary,

                        content:content,

                        createdAt:
                            new Date()
                            .toISOString()
                    })
            }
        );



    if(response.ok)
    {
        window.location.href="/";
    }
    else
    {
        alert("Failed creating post");
    }

}