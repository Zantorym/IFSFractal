#include "utils.h"

#define NUM_POINTS 1500000
#define WIN_WIDTH 1800
#define WIN_HEIGHT 900

std::vector<std::vector<double>> affineTransformations
{
  {0.5, 0.0, 0.0, 0.5, 0.0, 0.0},
  {0.5, 0.0, 0.0, 0.5, 1.28, 0.0},
  {0.5, 0.0, 0.0, 0.5, 0.64, 0.8}
};

std::vector<double> points;
bool pointsUpdated = false;

void generatePoints()
{
  points.clear();
  double point[2] = {1, 0.5};
  points.push_back(point[0]);
  points.push_back(point[1]);
  points.push_back(0.0);
  for(int i=0; i<NUM_POINTS; i++) {
    int j = rand()/((double)RAND_MAX + 1)*affineTransformations.size(); // Random index of affineTransformations
    double old_point[2] = {point[0], point[1]};
    point[0] = old_point[0]*affineTransformations[j][0] + old_point[1]*affineTransformations[j][1] + affineTransformations[j][4];
    point[1] = old_point[0]*affineTransformations[j][2] + old_point[1]*affineTransformations[j][3] + affineTransformations[j][5];
    points.push_back(point[0]);
    points.push_back(point[1]); 
    points.push_back(0.0);
  }
}

int main(int, char* argv[]) {
  GLFWwindow* window = setupWindow(WIN_WIDTH, WIN_HEIGHT);
  ImGuiIO& io = ImGui::GetIO(); // Create IO object

  ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

  unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
  glUseProgram(shaderProgram);

  // Create VBO, VAO
  unsigned int VBO_points;
  unsigned int VAO_points;
  glGenBuffers(1, &VBO_points);
  glGenVertexArrays(1, &VAO_points);

  generatePoints();
  glBindVertexArray(VAO_points);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_points);
  glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(GLfloat), &points[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0); //Enable first attribute buffer (vertices)

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static const char * items[]{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    static int numOfTrans = 0;
    static std::vector<std::vector<double>> trans;

    {
      ImGui::Begin("Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize);

      if (ImGui::Button("Generate")){
        std::cout<<"started generating"<<std::endl;
        affineTransformations = trans;
        pointsUpdated = true;
      }

      ImGui::Text("Affine transformations:");
      ImGui::Text("x1 = a*x + b*y + e");
      ImGui::Text("y1 = c*x + d*y + f");

      ImGui::Combo("Number of affine transformations", &numOfTrans, items, IM_ARRAYSIZE(items));

      trans.clear();
      for (int iter = 0; iter < numOfTrans+1; iter++) {
        static double a = 0;static double b = 0;static double c = 0;static double d = 0;static double e = 0;static double f = 0; 
        ImGui::Text("Number %d :", iter+1);
        ImGui::InputDouble("a", &a); ImGui::SameLine();
        ImGui::InputDouble("b", &b); ImGui::SameLine();
        ImGui::InputDouble("c", &c); ImGui::SameLine();
        ImGui::InputDouble("d", &d); ImGui::SameLine();
        ImGui::InputDouble("e", &e); ImGui::SameLine();
        ImGui::InputDouble("f", &f);
        trans.push_back({a, b, c, d, e, f});
      }

      ImGui::End();
    }

    // Rendering
    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    if (pointsUpdated) {
      // Update VAO/VBO for points (since we updated the affine transformations)
      generatePoints();
      glBindVertexArray(VAO_points);
      glBindBuffer(GL_ARRAY_BUFFER, VBO_points);
      glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(GLfloat), &points[0], GL_DYNAMIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0); //Enable first attribute buffer (vertices)

      pointsUpdated = false;



    }

    glUseProgram(shaderProgram);

    // Draw points
    glBindVertexArray(VAO_points);
    glPointSize(WIN_WIDTH*WIN_HEIGHT/NUM_POINTS);
    glDrawArrays(GL_POINTS, 0, points.size()/3); // Draw points

    glUseProgram(0);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // Delete VBO buffer
  glDeleteBuffers(1, &VBO_points);
  
  // Cleanup
  cleanup(window);
  return 0;

}