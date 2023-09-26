#include <iostream>

#include <PUJ_CGraf/AmbientLight.h>
#include <PUJ_CGraf/Controller.h>
#include <PUJ_CGraf/SphericalCamera.h>
#include <PUJ_CGraf/SpotLight.h>
#include <PUJ_CGraf/World.h>

#include <PUJ_CGraf/Object.h>
#include <PUJ_CGraf/Cylinder.h>
#include <PUJ_CGraf/Sphere.h>
#include <fstream>
#include <limits>
#include <sstream>
#include <map>
#include <tuple>
#include <boost/tokenizer.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional/optional.hpp>

/**
 */
class Stickman
    : public PUJ_CGraf::Object
{
public:
    using Self = Stickman;
    using Superclass = PUJ_CGraf::Object;
    using TReal = Superclass::TReal;
    using TNatural = Superclass::TNatural;
    using VReal = Superclass::VReal;
    using VNatural = Superclass::VNatural;

public:
    Stickman(const std::string &in_fname);
    virtual ~Stickman() = default;

    virtual void keypressed(unsigned char k) override
    {
        int arm_index = 0;
        if (this->m_Articulation < this->nJoints_arms)
            arm_index = this->nJoints_arms + this->m_Articulation;
        else if ((this->m_Articulation <= 2 * this->nJoints_arms) && (this->m_Articulation >= this->nJoints_arms))
            arm_index = -this->nJoints_arms + this->m_Articulation;
        if (std::isdigit(k))
            this->m_Articulation = k - '0';
        else if (k == 'x')
        {
            this->m_Joints_legs[this->m_Articulation].rotate(1, 1, 0, 0);
            this->m_Joints_arms[arm_index].rotate(1, -1, 0, 0);
        }
        else if (k == 'X')
        {
            this->m_Joints_legs[this->m_Articulation].rotate(1, -1, 0, 0);
            this->m_Joints_arms[arm_index].rotate(1, 1, 0, 0);
        }
        else if (k == 'y')
        {
            this->m_Joints_legs[this->m_Articulation].rotate(1, 0, 1, 0);
            this->m_Joints_arms[arm_index].rotate(1, 0, -1, 0);
        }
        else if (k == 'Y')
        {
            this->m_Joints_legs[this->m_Articulation].rotate(1, 0, -1, 0);
            this->m_Joints_arms[arm_index].rotate(1, 0, 1, 0);
        }
        else if (k == 'z')
        {
            this->m_Joints_legs[this->m_Articulation].rotate(1, 0, 0, 1);
            this->m_Joints_arms[arm_index].rotate(1, 0, 0, -1);
        }
        else if (k == 'Z')
        {
            this->m_Joints_legs[this->m_Articulation].rotate(1, 0, 0, -1);
            this->m_Joints_arms[arm_index].rotate(1, 0, 0, 1);
        }
    }
    virtual void draw() const override;

    void create_torso_head(float height,float radius,float head_radius);
    void create_arms(float height, float radius,float torso_height,float torso_radius);
    void create_legs(float height, float radius,float torso_radius);
    void set_from_json(const boost::property_tree::ptree &tree);
    

protected:
    int m_Articulation{0};
    unsigned int nJoints_arms {3};
    unsigned int nJoints_legs {3};
    PUJ_CGraf::Cylinder Torso;
    PUJ_CGraf::Sphere Head;
    std::vector<PUJ_CGraf::Sphere> m_Joints_arms;
    std::vector<PUJ_CGraf::Sphere> m_Joints_legs;
    std::vector<PUJ_CGraf::Cylinder> m_Arms;
    std::vector<PUJ_CGraf::Cylinder> m_Legs;
    boost::property_tree::ptree stickman_body;
    std::map<std::string, std::tuple<float, float>> bodyAtrributes;

};

// -------------------------------------------------------------------------
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Error: please provide the JSON path to draw the sitckman." << std::endl;
        return EXIT_FAILURE;
    }

    std::string jsonPath = argv[1];

    // Configure window
    PUJ_CGraf::Controller::set_shade_type_to_flat();
    PUJ_CGraf::Controller::init(&argc, argv, "Robotic Arm", 700, 700);
    // Configure world
    Stickman body(jsonPath);

    PUJ_CGraf::World world;
    world.add_child(&body);

    PUJ_CGraf::AmbientLight ambient_light;
    ambient_light.set_color(1, 1, 1);
    world.add_light(&ambient_light);

    PUJ_CGraf::SphericalCamera cam;
    world.set_camera(&cam);

    // Main loop
    PUJ_CGraf::Controller::set_world(&world);
    PUJ_CGraf::Controller::loop();

    return (EXIT_SUCCESS);
}

// -------------------------------------------------------------------------
Stickman::
    Stickman(const std::string &in_fname)
{

    std::ifstream in_file(in_fname.c_str());
    std::istringstream in_str(std::string(std::istreambuf_iterator<char>{in_file}, {}));
    in_file.close();

    try{
        boost::property_tree::read_json(in_str, this->stickman_body);
    }
    catch (const std::exception &err){
        std::cerr << "Error caught: " << err.what() << std::endl;
        // return EXIT_FAILURE;
    }
    if (this->stickman_body.empty()){
        std::cerr << "Error: Failed to read the JSON file or the file is empty" << std::endl;
    }
    set_from_json(this->stickman_body);

    float R_torso = std::get<0>(this->bodyAtrributes["Torso"]);
    float H_torso = std::get<1>(this->bodyAtrributes["Torso"]);

    float R_head = std::get<0>(this->bodyAtrributes["Head"]);

    float R_arms = std::get<0>(this->bodyAtrributes["Arms"]);
    float H_arms = std::get<1>(this->bodyAtrributes["Arms"]);

    float R_legs =  std::get<0>(this->bodyAtrributes["Legs"]);
    float H_legs = std::get<1>(this->bodyAtrributes["Legs"]); 

    create_torso_head(H_torso,R_torso,R_head);
    create_arms(H_arms,R_arms,H_torso,R_torso);
    create_legs(H_legs,R_legs,R_torso);

    this->m_BBox[0] = -50;
    this->m_BBox[1] = -50;
    this->m_BBox[2] = -50;
    this->m_BBox[3] = 50;
    this->m_BBox[4] = 50;
    this->m_BBox[5] = 50;
}

// -------------------------------------------------------------------------
void Stickman::
    draw() const
{
    this-> Torso.draw(); // torso is the general object and everybody depends on him
}

void Stickman::set_from_json(const boost::property_tree::ptree &tree)
{

    using _T = boost::property_tree::ptree;
    using _O = boost::optional<const _T &>;
    using _It = _T::const_iterator;
    for (_It i = tree.begin(); i != tree.end(); i++)
    {
        
        // get Head
        _O radius_node = i->second.get_child_optional("radius");
        _O height_node = i->second.get_child_optional("height");
        _O Joint_node = i->second.get_child_optional("nJoints");
        if (i->first == "Head")
            this->bodyAtrributes[i->first] = std::make_tuple(radius_node.get().get<float>(""), 0.0f);
        if (Joint_node)
        {
            if (i->first == "Arms")
            {
                this->nJoints_arms = Joint_node.get().get<unsigned int>("");
                this->m_Joints_arms.resize(2 * this->nJoints_arms);
                this->m_Arms.resize(2 * this->nJoints_arms);
                this->m_Joints_arms.shrink_to_fit();
                this->m_Arms.shrink_to_fit();
            }
            else if (i->first == "Legs")
            {
                this->nJoints_legs = Joint_node.get().get<unsigned int>("");
                this->m_Joints_legs.resize(2 * this->nJoints_legs);
                this->m_Legs.resize(2 * this->nJoints_legs);
                this->m_Joints_legs.shrink_to_fit();
                this->m_Legs.shrink_to_fit();
            }
        }
        if (radius_node && height_node)
            this->bodyAtrributes[i->first] = std::make_tuple(radius_node.get().get<float>(""), height_node.get().get<float>(""));
        set_from_json(i->second);
    }
}
// -------------------------------------------------------------------------
void Stickman::create_torso_head(float height,float radius,float head_radius)
{

    this->Torso.set_radius(radius);
    this->Torso.set_height(height);
    this->Torso.set_center(0, 0, 0);

    this->Head.set_radius(head_radius);
    this->Head.set_center(0, 0, 0);

    this->Head.sample(10, 10);
    this->Torso.sample(10, 10);
    
    this->Torso.add_child(&(this->Head),0,0,height+head_radius);

}
void Stickman::create_arms(float height, float radius,float torso_height,float torso_radius)
{

    for (unsigned int j = 0; j<this->nJoints_arms;j++)
    {
        this->m_Joints_arms[j].set_radius(radius);
        this->m_Joints_arms[j].set_center(0, 0, 0);
        

        this->m_Arms[j].set_center(0, 0, 0);
        this->m_Arms[j].set_radius(radius);
        this->m_Arms[j].set_height(height / this->nJoints_arms);


        this->m_Joints_arms[j].sample(10, 10);
        this->m_Arms[j].sample(10, 10);
        this->m_Joints_arms[j].add_child(&(this->m_Arms[j]));

        this->m_Joints_arms[j+this->nJoints_arms].set_radius(radius);
        this->m_Joints_arms[j+this->nJoints_arms].set_center(0, 0, 0);

        this->m_Arms[j+this->nJoints_arms].set_center(0, 0, 0);
        this->m_Arms[j+this->nJoints_arms].set_radius(radius);
        this->m_Arms[j+this->nJoints_arms].set_height(height / this->nJoints_arms);
        
        this->m_Joints_arms[j+this->nJoints_arms].sample(10, 10);
        this->m_Arms[j+this->nJoints_arms].sample(10, 10);
        this->m_Joints_arms[j+this->nJoints_arms].add_child(&(this->m_Arms[j+this->nJoints_arms]));

        if (j > 0)
        {
            this->m_Arms[j - 1]
                .add_child(&(this->m_Joints_arms[j]), 0, 0, height / this->nJoints_arms);
            this->m_Arms[j+this->nJoints_arms - 1]
                .add_child(&(this->m_Joints_arms[j+this->nJoints_arms]), 0, 0, height / this->nJoints_arms);
        }
        else
        {
            
            this->m_Arms[j].rotate(90, 1, 0, 0);
            this->m_Arms[j+this->nJoints_arms].rotate(90, 1, 0, 0);
            this->Torso.add_child(&(this->m_Joints_arms[j]), torso_radius, 0, 0.9 * torso_height);
            this->Torso.add_child(&(this->m_Joints_arms[j+this->nJoints_arms]), -torso_radius, 0, 0.9 * torso_height);
        }
    }
}
// -------------------------------------------------------------------------

void Stickman::create_legs(float height, float radius,float torso_radius)
{
    
    for (unsigned int j = 0; j<this->nJoints_legs;j++)
    {
        this->m_Joints_legs[j].set_radius(radius);
        this->m_Joints_legs[j].set_center(0, 0, 0);

        this->m_Legs[j].set_center(0, 0, 0);
        this->m_Legs[j].set_radius(radius);
        this->m_Legs[j].set_height(-height / this->nJoints_legs);

        this->m_Joints_legs[j].sample(10, 10);
        this->m_Legs[j].sample(10, 10);
        this->m_Joints_legs[j].add_child(&(this->m_Legs[j]));


        this->m_Joints_legs[j+this->nJoints_legs].set_radius(radius);
        this->m_Joints_legs[j+this->nJoints_legs].set_center(0, 0, 0);

        this->m_Legs[j+this->nJoints_legs].set_center(0, 0, 0);
        this->m_Legs[j+this->nJoints_legs].set_radius(radius);
        this->m_Legs[j+this->nJoints_legs].set_height(-height / this->nJoints_legs);

        this->m_Joints_legs[j+this->nJoints_legs].sample(10, 10);
        this->m_Legs[j+this->nJoints_legs].sample(10, 10);
        this->m_Joints_legs[j+this->nJoints_legs].add_child(&(this->m_Legs[j+this->nJoints_legs]));
        if (j > 0)
        {
            this->m_Legs[j - 1].add_child(&(this->m_Joints_legs[j]), 0, 0, -height / this->nJoints_legs);
            this->m_Legs[j+this->nJoints_legs - 1].add_child(&(this->m_Joints_legs[j+this->nJoints_legs]), 0, 0, -height / this->nJoints_legs);

        }
        else
        {
            this->Torso.add_child(&(this->m_Joints_legs[j]), torso_radius * 0.5, 0, 0);
            this->Torso.add_child(&(this->m_Joints_legs[j+this->nJoints_legs]), -torso_radius * 0.5, 0, 0);
        }
    }
}
// eof - Stickman.cxx
