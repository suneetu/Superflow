// Superflow.cxx
//

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "Superflow/Superflow.h"
#include "Superflow/StringTools.h"

using namespace std;

namespace sflow {

    Superflow::Superflow()
    {
        setAnaType(Ana_2Lep);
        setSelectTaus(true);

        m_runMode = SuperflowRunMode::null;

        m_passed = 0;
        m_weighted = 0;
        m_LambdaCutStore_Name_Exists = false;
        m_LambdaCutStoreUntitled = 1;

        m_countWeights = false;
        m_super_isData = false;

        m_outputFileName = "";
        m_outputFile = nullptr;
        m_HFT = nullptr;

        m_output_array = nullptr;
        m_HFT_array = nullptr;
        m_num_output_files = 1;

        m_period = ATLAS_period::null;
        m_stream = ATLAS_stream::null;

        m_varState = SupervarState::closed;
        m_superVar_hasFunction = false;
        m_superVar_hasNiceName = false;
        m_superVar_hasHFTName = false;
        m_superVar_Untitled = 1;

        m_mcWeighter = nullptr;

        m_nullExprFloat = [](Superlink* sl, var_float*) -> double { return 0.0; };
        m_nullExprDouble = [](Superlink* sl, var_double*) -> double { return 0.0; };
        m_nullExprInt = [](Superlink* sl, var_int*) -> int { return 0; };
        m_nullExprBool = [](Superlink* sl, var_bool*) -> bool { return false; };

        m_varFloat = nullptr;
        m_varDouble = nullptr;
        m_varInt = nullptr;
        m_varBool = nullptr;

        m_sysState = SupersysState::closed;
        m_sysTemplate.reset();
        m_sys_hasNiceName = false;
        m_sys_hasTreeName = false;
        m_sys_hasType = false;
        m_sys_hasSystematic = false;

        m_singleEventSyst = NtSys_NOM;
        m_RunSyst = new Supersys(SupersysType::central);

        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::A] = "periodA.physics_Egamma.PhysCont";
        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::B] = "periodB.physics_Egamma.PhysCont";
        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::C] = "periodC.physics_Egamma.PhysCont";
        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::D] = "periodD.physics_Egamma.PhysCont";
        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::E] = "periodE.physics_Egamma.PhysCont";
        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::G] = "periodG.physics_Egamma.PhysCont";
        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::H] = "periodH.physics_Egamma.PhysCont";
        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::I] = "periodI.physics_Egamma.PhysCont";
        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::J] = "periodJ.physics_Egamma.PhysCont";
        m_data_periods[ATLAS_stream::Egamma][ATLAS_period::L] = "periodL.physics_Egamma.PhysCont";

        m_data_periods[ATLAS_stream::Muons][ATLAS_period::A] = "periodA.physics_Muons.PhysCont";
        m_data_periods[ATLAS_stream::Muons][ATLAS_period::B] = "periodB.physics_Muons.PhysCont";
        m_data_periods[ATLAS_stream::Muons][ATLAS_period::C] = "periodC.physics_Muons.PhysCont";
        m_data_periods[ATLAS_stream::Muons][ATLAS_period::D] = "periodD.physics_Muons.PhysCont";
        m_data_periods[ATLAS_stream::Muons][ATLAS_period::E] = "periodE.physics_Muons.PhysCont";
        m_data_periods[ATLAS_stream::Muons][ATLAS_period::G] = "periodG.physics_Muons.PhysCont";
        m_data_periods[ATLAS_stream::Muons][ATLAS_period::H] = "periodH.physics_Muons.PhysCont";
        m_data_periods[ATLAS_stream::Muons][ATLAS_period::I] = "periodI.physics_Muons.PhysCont";
        m_data_periods[ATLAS_stream::Muons][ATLAS_period::J] = "periodJ.physics_Muons.PhysCont";
        m_data_periods[ATLAS_stream::Muons][ATLAS_period::L] = "periodL.physics_Muons.PhysCont";

        m_NtSys_to_string[NtSys_EES_Z_UP] = "EESZUP";
        m_NtSys_to_string[NtSys_EES_Z_DN] = "EESZDOWN";
        m_NtSys_to_string[NtSys_EES_MAT_UP] = "EESMATUP";
        m_NtSys_to_string[NtSys_EES_MAT_DN] = "EESMATDOWN";
        m_NtSys_to_string[NtSys_EES_PS_UP] = "EESPSUP";
        m_NtSys_to_string[NtSys_EES_PS_DN] = "EESPSDOWN";
        m_NtSys_to_string[NtSys_EES_LOW_UP] = "EESLOWUP";
        m_NtSys_to_string[NtSys_EES_LOW_DN] = "EESLOWDOWN";
        m_NtSys_to_string[NtSys_EER_UP] = "EERUP";
        m_NtSys_to_string[NtSys_EER_DN] = "EERDOWN";
        m_NtSys_to_string[NtSys_MS_UP] = "MSUP";
        m_NtSys_to_string[NtSys_MS_DN] = "MSDOWN";
        m_NtSys_to_string[NtSys_ID_UP] = "IDUP";
        m_NtSys_to_string[NtSys_ID_DN] = "IDDOWN";
        m_NtSys_to_string[NtSys_JES_UP] = "JESUP";
        m_NtSys_to_string[NtSys_JES_DN] = "JESDOWN";
        m_NtSys_to_string[NtSys_JER] = "JER";
        m_NtSys_to_string[NtSys_SCALEST_UP] = "SCALESTUP";
        m_NtSys_to_string[NtSys_SCALEST_DN] = "SCALESTDOWN";
        m_NtSys_to_string[NtSys_RESOST] = "RESOST";
        m_NtSys_to_string[NtSys_TRIGSF_EL_UP] = "TRIGSFELUP";
        m_NtSys_to_string[NtSys_TRIGSF_EL_DN] = "TRIGSFELDN";
        m_NtSys_to_string[NtSys_TRIGSF_MU_UP] = "TRIGSFMUUP";
        m_NtSys_to_string[NtSys_TRIGSF_MU_DN] = "TRIGSFMUDN";
        m_NtSys_to_string[NtSys_TES_UP] = "TESUP";
        m_NtSys_to_string[NtSys_TES_DN] = "TESDOWN";
        m_NtSys_to_string[NtSys_JVF_UP] = "JVFUP";
        m_NtSys_to_string[NtSys_JVF_DN] = "JVFDOWN";
    }

    Superflow::~Superflow()
    {}

    // CUT Operators
    // CUT Operators
    // CUT Operators

    Superflow& Superflow::operator<<(CutName cut_)
    {
        if (m_sysState == SupersysState::closed && m_varState == SupervarState::closed) {
            m_LambdaCutStore_Name_Exists = true;
            m_LambdaCutStoreNames.push_back(cut_.name);

            cout << app_name << "New cut: " << cut_.name << endl;
        }
        else {
            cout << app_name << "ERROR (Fatal): Cutflow operations are incorrectly ordered.";
            please exit(1);
        }
        return *this;
    }

    void Superflow::operator<<(std::function<bool(Superlink*)> cut_)
    {
        if (m_sysState == SupersysState::closed && m_varState == SupervarState::closed) {
            m_LambdaCutStore.push_back(cut_);

            if (m_LambdaCutStore_Name_Exists) {
                m_LambdaCutStore_Name_Exists = false;
            }
            else {
                m_LambdaCutStoreNames.push_back("Untitled-" + to_string(m_LambdaCutStoreUntitled));
                cout << app_name << "New cut: " << "Untitled-" << m_LambdaCutStoreUntitled << endl;
                m_LambdaCutStoreUntitled++;
            }

            m_RawCounter.push_back(0.0);
            m_WeightCounter.push_back(0.0);
        }
        else {
            cout << app_name << "ERROR (Fatal): Cutflow operations are incorrectly ordered.";
            please exit(1);
        }
    }

    // Var (HFT) Operators
    // Var (HFT) Operators
    // Var (HFT) Operators

    void Superflow::operator<<(NewVar new_var_name) // this is the NiceName
    {
        if (m_varState == SupervarState::closed && m_sysState == SupersysState::closed) {
            m_varNiceName.push_back(new_var_name.name);
            m_superVar_hasNiceName = true;
            m_varState = SupervarState::open;
        }
        else {
            cout << app_name << "ERROR (Fatal): Close the Var using SaveVar().";
            please exit(1);
        }
    }

    void Superflow::operator<<(HFTname hft_name)
    {
        if (m_varState == SupervarState::open && m_sysState == SupersysState::closed && !m_superVar_hasHFTName) {
            m_varHFTName.push_back(hft_name.name);
            m_superVar_hasHFTName = true;
        }
        else {
            cout << app_name << "ERROR (Fatal): Open a new Var using NewVar(string).";
            exit(1);
        }
    }

    void Superflow::operator<<(std::function<double(Superlink*, var_float*)> var_)
    {
        if (m_varState == SupervarState::open && m_sysState == SupersysState::closed && !m_superVar_hasFunction) {
            m_varExprFloat.push_back(var_); // fill
            m_varExprDouble.push_back(m_nullExprDouble);
            m_varExprInt.push_back(m_nullExprInt);
            m_varExprBool.push_back(m_nullExprBool);

            m_varType.push_back(SupervarType::sv_float);
            m_superVar_hasFunction = true;
        }
        else {
            cout << app_name << "ERROR (Fatal): First open a new Var using NewVar().";
            exit(1);
        }
    }

    void Superflow::operator<<(std::function<double(Superlink*, var_double*)> var_)
    {
        if (m_varState == SupervarState::open && m_sysState == SupersysState::closed && !m_superVar_hasFunction) {
            m_varExprFloat.push_back(m_nullExprFloat);
            m_varExprDouble.push_back(var_); // fill
            m_varExprInt.push_back(m_nullExprInt);
            m_varExprBool.push_back(m_nullExprBool);

            m_varType.push_back(SupervarType::sv_double);
            m_superVar_hasFunction = true;
        }
        else {
            cout << app_name << "ERROR (Fatal): First open a new Var using NewVar().";
            exit(1);
        }
    }

    void Superflow::operator<<(std::function<int(Superlink*, var_int*)> var_)
    {
        if (m_varState == SupervarState::open && m_sysState == SupersysState::closed && !m_superVar_hasFunction) {
            m_varExprFloat.push_back(m_nullExprFloat);
            m_varExprDouble.push_back(m_nullExprDouble);
            m_varExprInt.push_back(var_); // fill
            m_varExprBool.push_back(m_nullExprBool);

            m_varType.push_back(SupervarType::sv_int);
            m_superVar_hasFunction = true;
        }
        else {
            cout << app_name << "ERROR (Fatal): First open a new Var using NewVar().";
            exit(1);
        }
    }

    void Superflow::operator<<(std::function<bool(Superlink*, var_bool*)> var_)
    {
        if (m_varState == SupervarState::open && m_sysState == SupersysState::closed && !m_superVar_hasFunction) {
            m_varExprFloat.push_back(m_nullExprFloat);
            m_varExprDouble.push_back(m_nullExprDouble);
            m_varExprInt.push_back(m_nullExprInt);
            m_varExprBool.push_back(var_); // fill

            m_varType.push_back(SupervarType::sv_bool);
            m_superVar_hasFunction = true;
        }
        else {
            cout << app_name << "ERROR (Fatal): First open a new Var using NewVar().";
            exit(1);
        }
    }

    void Superflow::operator<<(SaveVar save_var)
    {
        if (m_varState == SupervarState::open && m_superVar_hasFunction && m_superVar_hasNiceName) {
            if (!m_superVar_hasHFTName) {
                m_varHFTName.push_back("Untitled_" + to_string(m_superVar_Untitled));
                m_superVar_Untitled++;
                m_superVar_hasHFTName = true;
            }

            m_varState = SupervarState::closed;
            m_superVar_hasFunction = false;
            m_superVar_hasNiceName = false;
            m_superVar_hasHFTName = false;

            cout << app_name << "New var: " << m_varNiceName.back() << endl;
            cout << app_name << "    HFT: " << m_varHFTName.back() << endl;
        }
        else {
            if (m_varState != SupervarState::open) {
                cout << app_name << "ERROR (Fatal): First open a new Var using NewVar().";
            }
            else {
                cout << app_name << "ERROR (Fatal): A lambda-expression is required.";
            }
            exit(1);
        }
    }

    // Systematics Operators
    // Systematics Operators
    // Systematics Operators

    void Superflow::operator<<(NewSystematic new_sys) // this is the NiceName
    {
        if (m_sysState == SupersysState::closed && m_varState == SupervarState::closed) {
            m_sysTemplate.name = new_sys.name;
            m_sys_hasNiceName = true;
            m_sysState = SupersysState::open;
        }
        else {
            cout << app_name << "ERROR (Fatal): Close using SaveSystematic().";
            exit(1);
        }
    }

    void Superflow::operator<<(TreeName tree_name)
    {
        if (m_sysState == SupersysState::open && m_varState == SupervarState::closed && !m_sys_hasTreeName) {
            m_sysTemplate.tree_name = tree_name.name;
            m_sys_hasTreeName = true;
        }
        else {
            cout << app_name << "ERROR (Fatal): Open a NewSystematic().";
            exit(1);
        }
    }

    void Superflow::operator<<(EventSystematic obj_)
    {
        if (m_sysState == SupersysState::open && m_varState == SupervarState::closed && !m_sys_hasSystematic) {
            m_sysTemplate.event_syst = obj_.event_syst_;
            m_sysTemplate.weight_syst = SupersysWeight::null;
            m_sys_hasSystematic = true;

            m_sysTemplate.type = SupersysType::event;
            m_sys_hasType = true;
        }
        else {
            cout << app_name << "ERROR (Fatal): Open a NewSystematic().";
            exit(1);
        }
    }

    void Superflow::operator<<(WeightSystematic obj_)
    {
        if (m_sysState == SupersysState::open && m_varState == SupervarState::closed && !m_sys_hasSystematic) {
            m_sysTemplate.event_syst = NtSys_NOM;
            m_sysTemplate.weight_syst_up = obj_.weight_syst_up;
            m_sysTemplate.weight_syst_down = obj_.weight_syst_down;
            m_sys_hasSystematic = true;

            m_sysTemplate.type = SupersysType::weight;
            m_sys_hasType = true;
        }
        else {
            cout << app_name << "ERROR (Fatal): Open a NewSystematic().";
            exit(1);
        }
    }

    void Superflow::operator<<(SaveSystematic save_var)
    {
        if (m_sysState == SupersysState::open
            && m_varState == SupervarState::closed
            && m_sys_hasNiceName
            && m_sys_hasTreeName
            && m_sys_hasType
            && m_sys_hasSystematic
            ) {
            if (m_sysTemplate.name == "") m_sysTemplate.name = m_sysTemplate.tree_name;

            m_sysState = SupersysState::closed;
            m_sys_hasNiceName = false;
            m_sys_hasTreeName = false;
            m_sys_hasSystematic = false;
            m_sys_hasType = false;

            m_sysStore.push_back(m_sysTemplate);
            m_sysTemplate.reset();

            cout << app_name << "New systematic: " << m_sysStore.back().name << endl;
            if (m_sysStore.back().type == SupersysType::event) {
                cout << app_name << "    event systematic: " << m_sysStore.back().tree_name << endl;
            }
            else if (m_sysStore.back().type == SupersysType::weight) {
                cout << app_name << "    weight systematics: " << m_sysStore.back().tree_name << weight_suffix_up
                    << "/" << m_sysStore.back().tree_name << weight_suffix_down << endl;
            }
            else {
                cout << app_name << "ERROR (Fatal): Impossible SupersysType.";
                exit(1);
            }

        }
        else {
            if (m_sysState != SupersysState::open) {
                cout << app_name << "ERROR (Fatal): First open a NewSystematic().";
            }
            else {
                cout << app_name << "ERROR (Fatal): Can't save incomplete systematic object.";
            }
            exit(1);
        }
    }

    // Superlink
    // Superlink
    // Superlink

    void Superflow::attach_superlink(Superlink* sl_)
    {
        sl_->anaType = m_anaType;

        sl_->nt = &nt; // SusyNt
        sl_->weights = m_weights;
        sl_->nt_sys = m_RunSyst->event_syst;

        sl_->baseLeptons = &m_baseLeptons;
        sl_->baseElectrons = &m_baseElectrons;
        sl_->baseMuons = &m_baseMuons;
        sl_->baseTaus = &m_baseTaus;
        sl_->baseJets = &m_baseJets;

        sl_->leptons = &m_signalLeptons;
        sl_->electrons = &m_signalElectrons;
        sl_->muons = &m_signalMuons;
        sl_->taus = &m_signalTaus;
        sl_->jets = &m_signalJets;

        sl_->met = m_met;

        sl_->dileptonTrigger = m_trigObj;

        sl_->jvfTool = m_jvfTool;

        if (nt.evt()->isMC) {
            sl_->isMC = true;
        }
        else {
            sl_->isData = true;
        }
    }

    // TSelector States
    // TSelector States
    // TSelector States

    void Superflow::Begin(TTree* /*tree*/)
    {
        cout << app_name << m_sample << endl;
        cout << app_name << m_sample << endl;
        cout << app_name << m_sample << endl;

        if (m_runMode == SuperflowRunMode::null) {
            cout << app_name << "ERROR (Fatal): Missing call to Superflow::setRunMode()." << endl;
            exit(1);
        }

        if (m_varState != SupervarState::closed || m_sysState != SupersysState::closed) {
            cout << app_name << "ERROR (Fatal): Close the Var using SaveVar()." << endl;;
            exit(1);
        }

        if (m_runMode == SuperflowRunMode::single_event_syst && m_singleEventSyst == NtSys_NOM) {
            cout << app_name << "ERROR (Fatal): SuperflowRunMode::single_event_syst: Call setSingleEventSyst(SusyNtSys nt_syst_)." << endl;
            exit(1);
        }

        // determine number of weight systematics
        if (m_runMode == SuperflowRunMode::nominal_and_weight_syst || m_runMode == SuperflowRunMode::all_syst) {
            for (int i = 0; i < m_sysStore.size(); i++) {
                if (m_sysStore[i].type == SupersysType::weight) {
                    index_weight_sys.push_back(i);
                }
            }
        }

        // determine number of event systematics
        if (m_runMode == SuperflowRunMode::all_syst) {
            for (int i = 0; i < m_sysStore.size(); i++) {
                if (m_sysStore[i].type == SupersysType::weight) {
                    index_weight_sys.push_back(i);
                }
            }
        }

        // Set correct event systematic
        if (m_runMode == SuperflowRunMode::single_event_syst) {
            for (int i = 0; i < m_sysStore.size(); i++) {
                if (m_sysStore[i].type == SupersysType::event && m_sysStore[i].event_syst == m_singleEventSyst) {
                    delete m_RunSyst;

                    m_RunSyst = &m_sysStore[i];
                }
            }
        }

        cout << app_name << "in Begin()" << endl;
        SusyNtAna::Begin(0);
        string period = "Moriond";
        bool useReweightUtils = false;
        m_trigObj = new DilTrigLogic(period, useReweightUtils);
    }

    void Superflow::Init(TTree* tree)
    {
        cout << app_name << "in Init()" << endl;
        SusyNtAna::Init(tree);

        if (!nt.evt()->isMC) {
            m_runMode = SuperflowRunMode::data; // Changing the run mode!!
        }
        else {
            initMcWeighter(tree);
        }

        // output file name
        stringstream sfile_name_;

        // determine output file name
        if (m_runMode == SuperflowRunMode::data) {
            m_countWeights = false;
            m_super_isData = true;

            sfile_name_ << "NOM_";

            size_t find_period = m_sample.find("period");
            if (find_period != string::npos) {
                char data_per_ = m_sample.substr(find_period + 6, 1)[0];

                if (data_per_ >= 'A') {
                    ATLAS_period this_period = static_cast<ATLAS_period>(data_per_ - 'A');
                    cout << app_name << "Determined data period: " << data_per_ << endl;

                    m_period = this_period; // set the period

                    size_t find_Egamma = m_sample.find("Egamma");
                    size_t find_Muons = m_sample.find("Muons");

                    if (find_Egamma != string::npos) {
                        cout << app_name << "Determined stream: Egamma" << endl;
                        m_stream = ATLAS_stream::Egamma;
                    }
                    else if (find_Muons != string::npos) {
                        cout << app_name << "Determined stream: Muons" << endl;
                        m_stream = ATLAS_stream::Muons;
                    }
                    else {
                        cout << app_name << "ERROR (Fatal): Unknown data stream! Supported streams: Egamma, Muons." << endl;
                        exit(1);
                    }
                }

                sfile_name_ << m_data_periods[m_stream][m_period] << ".root";
                cout << app_name << "Setting output file name to: " << sfile_name_.str() << endl;

                m_outputFileName = sfile_name_.str();
            }
            else {
                cout << app_name << "ERROR (fatal): Failed to determine data period. Try to specify the full path." << endl;
                exit(1);
            }
        }
        else if (m_runMode == SuperflowRunMode::single_event_syst) {
            if (m_NtSys_to_string.count(m_singleEventSyst) != 0) {
                sfile_name_ << m_NtSys_to_string[m_singleEventSyst] << "_" << nt.evt()->mcChannel << ".root";
                cout << app_name << "Run mode: SuperflowRunMode::single_event_syst" << endl;
                cout << app_name << "Setting output file name to: " << sfile_name_.str() << endl;
                m_outputFileName = sfile_name_.str();
            }
            else {
                cout << app_name << "ERROR (Fatal): Unknown event systematic! Code: " << static_cast<int>(m_singleEventSyst) << endl;
                exit(1);
            }
        }
        else if (nt.evt()->isMC) {
            sfile_name_ << "NOM_" << nt.evt()->mcChannel << ".root";

            cout << app_name << "Run mode: SuperflowRunMode::nominal_and_weight_syst" << endl;
            cout << app_name << "Setting output file name to: " << sfile_name_.str() << endl;
            m_outputFileName = sfile_name_.str();
        }
        else {
            cout << app_name << "ERROR (Fatal): Inconsistent setup." << endl;
            exit(1);
        }

        m_outputFile = new TFile(m_outputFileName.data(), "RECREATE");

        // output tree name
        stringstream tree_name;
        if (m_runMode == SuperflowRunMode::data) {
            tree_name << "id_" << m_data_periods[m_stream][m_period];
        }
        else {
            tree_name << "id_" << nt.evt()->mcChannel;
        }

        cout << app_name << "Tree name: " << tree_name.str() << endl;

        // initialize output tree
        m_HFT = new TTree(tree_name.str().data(), tree_name.str().data());
        m_HFT->SetDirectory(0); // make tree "memory resident"

        // define number of trees
        m_tree_leafs_size = m_varType.size() + 2 * index_weight_sys.size();// 2nd term may be zero
        m_weight_leaf_offset = m_varType.size();

        m_varFloat = new Float_t[m_tree_leafs_size]; // this one is larger to hold the syst_WEIGHT
        m_varDouble = new Double_t[m_varType.size()];
        m_varInt = new Int_t[m_varType.size()];
        m_varBool = new Bool_t[m_varType.size()];

        // initialize HFT
        for (int v_ = 0; v_ < m_tree_leafs_size; v_++) m_varFloat[v_] = 1.0;
        for (int v_ = 0; v_ < m_varType.size(); v_++) m_varDouble[v_] = 1.0;
        for (int v_ = 0; v_ < m_varType.size(); v_++) m_varInt[v_] = 0;
        for (int v_ = 0; v_ < m_varType.size(); v_++) m_varBool[v_] = false;

        for (int v_ = 0; v_ < m_varType.size(); v_++) {
            switch (m_varType[v_]) {
                case SupervarType::sv_float: {
                    string leaflist_ = m_varHFTName[v_] + "/F";
                    m_HFT->Branch(m_varHFTName[v_].data(), m_varFloat + v_, leaflist_.data());
                    break;
                }
                case SupervarType::sv_double: {
                    string leaflist_ = m_varHFTName[v_] + "/D";
                    m_HFT->Branch(m_varHFTName[v_].data(), m_varDouble + v_, leaflist_.data());
                    break;
                }
                case SupervarType::sv_int: {
                    string leaflist_ = m_varHFTName[v_] + "/I";
                    m_HFT->Branch(m_varHFTName[v_].data(), m_varInt + v_, leaflist_.data());
                    break;
                }
                case SupervarType::sv_bool: {
                    string leaflist_ = m_varHFTName[v_] + "/O";
                    m_HFT->Branch(m_varHFTName[v_].data(), m_varBool + v_, leaflist_.data());
                    break;
                }
            }
        }

        for (int w_ = 0; w_ < index_weight_sys.size(); w_++) {
            string syst_var_name_up = weight_prefix + m_sysStore[index_weight_sys[w_]].tree_name + weight_suffix_up;
            string syst_var_name_down = weight_prefix + m_sysStore[index_weight_sys[w_]].tree_name + weight_suffix_down;

            string leaflist_up = syst_var_name_up + "/F";
            string leaflist_down = syst_var_name_down + "/F";

            cout << app_name << "Weight var trees: " << syst_var_name_up << ", " << syst_var_name_down << endl;

            m_HFT->Branch(syst_var_name_up.data(), m_varFloat + m_weight_leaf_offset + 2 * w_, leaflist_up.data());
            m_HFT->Branch(syst_var_name_down.data(), m_varFloat + m_weight_leaf_offset + 2 * w_ + 1, leaflist_down.data());
        }
    }


    Bool_t Superflow::Process(Long64_t entry)
    {
        GetEntry(entry);
        m_chainEntry++; // SusyNtAna counter

        if (m_chainEntry % 50000 == 0) {
            cout << app_name << "**** Processing entry " << setw(6) << m_chainEntry
                << " run " << setw(6) << nt.evt()->run
                << " event " << setw(7) << nt.evt()->event << " ****" << endl;
        }

        // select baseline and signal objects
        bool removeLepsFromIso = false;

        // these are flags
        var_float* vf_ = nullptr;
        var_double* vd_ = nullptr;
        var_int* vi_ = nullptr;
        var_bool* vb_ = nullptr;

        switch (m_runMode) {
            case SuperflowRunMode::data: {
                clearObjects();
                selectObjects(m_RunSyst->event_syst, removeLepsFromIso, TauID_medium); // always select with nominal? (to compute event flags)

                EventFlags eventFlags = computeEventFlags();
                if (eventFlags.passAllEventCriteria()) {

                    m_weights = new Superweight();
                    Superlink* sl_ = new Superlink;
                    attach_superlink(sl_);

                    bool pass_cuts = true; // loop over and appply the cuts in m_CutStore.
                    if (m_LambdaCutStore.size() > 0) {
                        for (int i = 0; i < m_LambdaCutStore.size(); i++) {
                            pass_cuts = m_LambdaCutStore[i](sl_); // run the cut function

                            if (pass_cuts) {
                                m_RawCounter[i]++;
                            }
                            else {
                                break;
                            }
                        }
                    }

                    if (pass_cuts) { // data passed cuts, so fill HFTs.
                        for (int v_ = 0; v_ < m_varType.size(); v_++) {
                            switch (m_varType[v_]) {
                                case SupervarType::sv_float: {
                                    m_varFloat[v_] = m_varExprFloat[v_](sl_, vf_); break;
                                }
                                case SupervarType::sv_double: {
                                    m_varDouble[v_] = m_varExprDouble[v_](sl_, vd_); break;
                                }
                                case SupervarType::sv_int: {
                                    m_varInt[v_] = m_varExprInt[v_](sl_, vi_); break;
                                }
                                case SupervarType::sv_bool: {
                                    m_varBool[v_] = m_varExprBool[v_](sl_, vb_); break;
                                }
                            }
                        }
                        m_HFT->Fill();
                    }
                    delete sl_;
                    delete m_weights;
                }
            } break;
            case SuperflowRunMode::nominal:
            case SuperflowRunMode::single_event_syst: {
                clearObjects();
                selectObjects(m_RunSyst->event_syst, removeLepsFromIso, TauID_medium); // always select with nominal? (to compute event flags)

                EventFlags eventFlags = computeEventFlags();
                if (eventFlags.passAllEventCriteria()) {

                    m_weights = new Superweight();
                    Superlink* sl_ = new Superlink;
                    attach_superlink(sl_);

                    bool pass_cuts = true; // loop over and appply the cuts in m_CutStore.

                    if (m_countWeights) {
                        assignNonStaticWeightComponents(nt, *m_mcWeighter, m_signalLeptons, m_baseJets, m_RunSyst, m_weights);
                    }

                    if (m_LambdaCutStore.size() > 0) {
                        for (int i = 0; i < m_LambdaCutStore.size(); i++) {
                            pass_cuts = m_LambdaCutStore[i](sl_); // run the cut function

                            if (pass_cuts) {
                                m_RawCounter[i]++;
                                if (m_countWeights) m_WeightCounter[i] += m_weights->product();
                            }
                            else {
                                break;
                            }
                        }
                    }

                    if (pass_cuts) {
                        if (!m_countWeights) {
                            assignNonStaticWeightComponents(nt, *m_mcWeighter, m_signalLeptons, m_baseJets, m_RunSyst, m_weights);
                            m_WeightCounter[m_LambdaCutStore.size() - 1] += m_weights->product();
                        }

                        // FILL_HFTs
                        for (int v_ = 0; v_ < m_varType.size(); v_++) {
                            switch (m_varType[v_]) {
                                case SupervarType::sv_float: {
                                    m_varFloat[v_] = m_varExprFloat[v_](sl_, vf_); break;
                                }
                                case SupervarType::sv_double: {
                                    m_varDouble[v_] = m_varExprDouble[v_](sl_, vd_); break;
                                }
                                case SupervarType::sv_int: {
                                    m_varInt[v_] = m_varExprInt[v_](sl_, vi_); break;
                                }
                                case SupervarType::sv_bool: {
                                    m_varBool[v_] = m_varExprBool[v_](sl_, vb_); break;
                                }
                            }
                        }
                        m_HFT->Fill();
                    }
                    delete sl_;
                    delete m_weights;
                }
            } break;
            case SuperflowRunMode::all_syst: {
                clearObjects();
                selectObjects(m_RunSyst->event_syst, removeLepsFromIso, TauID_medium); // always select with nominal? (to compute event flags)

                EventFlags eventFlags = computeEventFlags();
                if (eventFlags.passAllEventCriteria()) {

                    m_weights = new Superweight();
                    Superlink* sl_ = new Superlink;
                    attach_superlink(sl_);

                    bool pass_cuts = true; // loop over and appply the cuts in m_CutStore.

                    if (m_countWeights) {
                        assignNonStaticWeightComponents(nt, *m_mcWeighter, m_signalLeptons, m_baseJets, m_RunSyst, m_weights);
                    }

                    if (m_LambdaCutStore.size() > 0) {
                        for (int i = 0; i < m_LambdaCutStore.size(); i++) {
                            pass_cuts = m_LambdaCutStore[i](sl_); // run the cut function

                            if (pass_cuts) {
                                m_RawCounter[i]++;
                                if (m_countWeights) m_WeightCounter[i] += m_weights->product();
                            }
                            else {
                                break;
                            }
                        }
                    }

                    if (pass_cuts) {
                        if (!m_countWeights) {
                            assignNonStaticWeightComponents(nt, *m_mcWeighter, m_signalLeptons, m_baseJets, m_RunSyst, m_weights);
                            m_WeightCounter[m_LambdaCutStore.size() - 1] += m_weights->product();
                        }

                        // FILL_HFTs
                        for (int v_ = 0; v_ < m_varType.size(); v_++) {
                            switch (m_varType[v_]) {
                                case SupervarType::sv_float: {
                                    m_varFloat[v_] = m_varExprFloat[v_](sl_, vf_); break;
                                }
                                case SupervarType::sv_double: {
                                    m_varDouble[v_] = m_varExprDouble[v_](sl_, vd_); break;
                                }
                                case SupervarType::sv_int: {
                                    m_varInt[v_] = m_varExprInt[v_](sl_, vi_); break;
                                }
                                case SupervarType::sv_bool: {
                                    m_varBool[v_] = m_varExprBool[v_](sl_, vb_); break;
                                }
                            }
                        }
                        m_HFT->Fill();
                    }
                    delete sl_;
                    delete m_weights;
                }
            } break;
            case SuperflowRunMode::nominal_and_weight_syst: {
                delete m_RunSyst;
                m_RunSyst = new Supersys(SupersysType::central);

                clearObjects();
                selectObjects(m_RunSyst->event_syst, removeLepsFromIso, TauID_medium); // always select with nominal? (to compute event flags)

                EventFlags eventFlags = computeEventFlags();
                if (eventFlags.passAllEventCriteria()) {

                    m_weights = new Superweight();
                    Superlink* sl_ = new Superlink;
                    attach_superlink(sl_);

                    bool pass_cuts = true; // loop over and appply the cuts in m_CutStore.

                    if (m_LambdaCutStore.size() > 0) {
                        for (int i = 0; i < m_LambdaCutStore.size(); i++) {
                            pass_cuts = m_LambdaCutStore[i](sl_); // run the cut function

                            if (pass_cuts) {
                                m_RawCounter[i]++;
                            }
                            else {
                                break;
                            }
                        }
                    }

                    if (pass_cuts) {
                        assignNonStaticWeightComponents(nt, *m_mcWeighter, m_signalLeptons, m_baseJets, m_RunSyst, m_weights);

                        double nom_eventweight = m_weights->product();
                        m_WeightCounter[m_LambdaCutStore.size() - 1] += m_weights->product();

                        // FILL HFTs
                        for (int v_ = 0; v_ < m_varType.size(); v_++) {
                            switch (m_varType[v_]) {
                                case SupervarType::sv_float: {
                                    m_varFloat[v_] = m_varExprFloat[v_](sl_, vf_); break;
                                }
                                case SupervarType::sv_double: {
                                    m_varDouble[v_] = m_varExprDouble[v_](sl_, vd_); break;
                                }
                                case SupervarType::sv_int: {
                                    m_varInt[v_] = m_varExprInt[v_](sl_, vi_); break;
                                }
                                case SupervarType::sv_bool: {
                                    m_varBool[v_] = m_varExprBool[v_](sl_, vb_); break;
                                }
                            }
                        }

                        // FILL more HFTs
                        for (int w_ = 0; w_ < index_weight_sys.size(); w_++) {
                            Superweight* weightComponents_copy = new Superweight(*m_weights);

                            // Up variation
                            m_RunSyst->weight_syst = m_sysStore[index_weight_sys[w_]].weight_syst_up; // do up variation
                            assignNonStaticWeightComponents(nt, *m_mcWeighter, m_signalLeptons, m_baseJets, m_RunSyst, weightComponents_copy);
                            double up_weight = weightComponents_copy->product();
                            delete weightComponents_copy;

                            weightComponents_copy = new Superweight(*m_weights);
                            // Down variation
                            m_RunSyst->weight_syst = m_sysStore[index_weight_sys[w_]].weight_syst_down; // do down variation
                            assignNonStaticWeightComponents(nt, *m_mcWeighter, m_signalLeptons, m_baseJets, m_RunSyst, weightComponents_copy);
                            double down_weight = weightComponents_copy->product();

                            delete weightComponents_copy;
                            weightComponents_copy = nullptr;

                            if (nom_eventweight > m_epsilon && up_weight < nom_eventweight && down_weight > nom_eventweight) {
                                double temp = up_weight; // reason to swap these
                                up_weight = down_weight;
                                down_weight = temp;
                            }

                            if (nom_eventweight > m_epsilon) {
                                up_weight = up_weight / nom_eventweight;
                                down_weight = down_weight / nom_eventweight;
                            }
                            else {
                                up_weight = 1.0;
                                down_weight = 1.0;
                            }

                            // cout << app_name << "Weight variation: " << m_sysStore[index_weight_sys[w_]].tree_name << endl;
                            // cout << "    nom: " << nom_eventweight << endl;
                            // cout << "    up : " << up_weight << endl;
                            // cout << "    dwn: " << down_weight << endl;
                            *(m_varFloat + m_weight_leaf_offset + 2 * w_) = up_weight;
                            *(m_varFloat + m_weight_leaf_offset + 2 * w_ + 1) = down_weight;
                        } // cout << app_name << endl << app_name << endl;

                        m_RunSyst->weight_syst = SupersysWeight::null; // must reset this value!
                        m_HFT->Fill();
                    }
                    delete sl_;
                    delete m_weights;
                }
            } break;
            case SuperflowRunMode::null: {
                cout << app_name << "ERROR (fatal): SuperflowRunMode::null seen in event loop." << endl;
                exit(1);
            } break;
        }

        delete vf_;
        delete vd_;
        delete vi_;
        delete vb_;

        return kTRUE;
    }

    void Superflow::Terminate()
    {
        cout << app_name << "in Terminate()" << endl;

        cout << app_name << "Raw" << endl;
        cout << app_name << "Raw" << endl;
        cout << app_name << "Raw" << endl;
        for (int i = 0; i < m_LambdaCutStore.size(); i++) {
            cout << app_name << "Cut " << pad_width(to_string(i), 2) << ": " << pad_width(m_LambdaCutStoreNames[i], 32) << ": " << m_RawCounter[i] << endl;
        }
        cout << app_name << endl << app_name << endl;

        cout << app_name << "Weighted" << endl;
        cout << app_name << "Weighted" << endl;
        cout << app_name << "Weighted" << endl;
        for (int i = 0; i < m_LambdaCutStore.size(); i++) {
            cout << app_name << "Cut " << pad_width(to_string(i), 2) << ": " << pad_width(m_LambdaCutStoreNames[i], 32) << ": " << m_WeightCounter[i] << endl;
        }
        cout << app_name << endl << app_name << endl;

        m_outputFile->WriteTObject(m_HFT);
        m_outputFile->Close();

        cout << app_name << "Files OK." << endl;

        SusyNtAna::Terminate();
        if (m_mcWeighter) delete m_mcWeighter;

        delete[] m_varFloat;
        delete[] m_varDouble;
        delete[] m_varInt;
        delete[] m_varBool;

        if (m_runMode != SuperflowRunMode::single_event_syst) delete m_RunSyst;

        cout << app_name << "Done." << endl;
    }

    bool Superflow::initMcWeighter(TTree *tree)
    {
        bool success = false;
        if (tree) {
            string xsecDir = gSystem->ExpandPathName("$ROOTCOREBIN/data/SUSYTools/mc12_8TeV/");
            m_mcWeighter = new MCWeighter(tree, xsecDir);

            bool isPmssmSample = false;
            if (sampleName().find("Herwigpp_UEEE3_CTEQ6L1_DGnoSL_TB10") != string::npos) {
                isPmssmSample = true;
            }

            m_mcWeighter->parseAdditionalXsecFile("${ROOTCOREBIN}/data/Superflow/LFV.txt", /*m_dbg*/ false);

            if (isPmssmSample) {
                m_mcWeighter->setLabelBinCounter("Initial").clearAndRebuildSumwMap(m_tree);
            }
            if (m_dbg) {
                cout << app_name << "MCWeighter has been initialized." << endl;
            }
        }
        else {
            cout << app_name << "ERROR: Invalid input tree, cannot initialize MCWeighter." << endl;
        }
        return success;
    }

    bool Superflow::assignNonStaticWeightComponents(
        Susy::SusyNtObject &ntobj,
        MCWeighter &weighter,
        const LeptonVector& leptons,
        const JetVector& jets,
        Supersys* super_sys,
        Superweight* weights_
        )
    {
        // MCWeighter's susynt-weight calculation
        if (ntobj.evt()->isMC) {
            MCWeighter::WeightSys wSys = MCWeighter::Sys_NOM;

            if (super_sys->weight_syst == SupersysWeight::null) {
                weights_->susynt = weighter.getMCWeight(ntobj.evt(), LUMI_A_L, wSys);
            }
            else {
                bool do_susynt_w = false;

                switch (super_sys->weight_syst) {
                    case SupersysWeight::XSUP: {
                        wSys = MCWeighter::Sys_XSEC_UP;
                        do_susynt_w = true;
                        break;
                    }
                    case SupersysWeight::XSDOWN: {
                        wSys = MCWeighter::Sys_XSEC_DN;
                        do_susynt_w = true;
                        break;
                    }
                    case SupersysWeight::PILEUPUP: {
                        wSys = MCWeighter::Sys_PILEUP_UP;
                        do_susynt_w = true;
                        break;
                    }
                    case SupersysWeight::PILEUPDOWN: {
                        wSys = MCWeighter::Sys_PILEUP_DN;
                        do_susynt_w = true;
                        break;
                    }
                    default: break;
                }
                if (do_susynt_w) {
                    weights_->susynt = weighter.getMCWeight(ntobj.evt(), LUMI_A_L, wSys);
                }
            }

            // Other weight systematic variations
            if (leptons.size() > 1) {
                // vars.hasFiredTrig = m_trigObj->passDilEvtTrig(leptons, m_met->Et, nt.evt());
                // vars.hasTrigMatch = m_trigObj->passDilTrigMatch(leptons, m_met->Et, nt.evt());
                const Lepton &l0 = *(leptons[0]);
                const Lepton &l1 = *(leptons[1]);

                bool do_lepSf_ = false; // do_lepSf_

                switch (super_sys->weight_syst) {
                    case SupersysWeight::ESFUP:
                    case SupersysWeight::ESFDOWN:
                    case SupersysWeight::MEFFUP:
                    case SupersysWeight::MEFFDOWN:
                    case SupersysWeight::null: {
                        do_lepSf_ = true;
                    } break;
                    default: break;
                }
                if (do_lepSf_) {
                    weights_->lepSf = (computeLeptonEfficiencySf(l0, super_sys->weight_syst) * computeLeptonEfficiencySf(l1, super_sys->weight_syst));
                }

                if (super_sys->weight_syst == SupersysWeight::null) { // do trigger // Consider if this is safe. (it is)
                    weights_->trigger = computeDileptonTriggerWeight(leptons, super_sys->event_syst);
                }

                bool do_btag_ = false; // do_btag_

                switch (super_sys->weight_syst) {
                    case SupersysWeight::BJETUP:
                    case SupersysWeight::BJETDOWN:
                    case SupersysWeight::CJETUP:
                    case SupersysWeight::CJETDOWN:
                    case SupersysWeight::BMISTAGUP:
                    case SupersysWeight::BMISTAGDOWN:
                    case SupersysWeight::null: {
                        do_btag_ = true;
                    } break;
                    default: break;
                }
                if (do_btag_) {
                    weights_->btag = computeBtagWeight(jets, nt.evt(), super_sys->weight_syst);
                }
            }
        }

        return true;
    }

    double Superflow::computeDileptonTriggerWeight(const LeptonVector &leptons, const SusyNtSys sys)
    {
        double trigW = 1.0;
        if (leptons.size() == 2) {

            trigW = m_trigObj->getTriggerWeight(leptons, nt.evt()->isMC, m_met->Et, m_signalJets2Lep.size(), nt.evt()->nVtx, sys);

            bool twIsInvalid = !(trigW >= 0) || trigW < 0.0;

            if (twIsInvalid) {
                // if (m_dbg) cout << app_name << "computeDileptonTriggerWeight: invalid weight " << trigW << ", using 0.0" << endl;
                trigW = (twIsInvalid ? 0.0 : trigW);
            }
        }
        return trigW;
    }


    double Superflow::computeBtagWeight(const JetVector& jets, const Susy::Event* evt, SupersysWeight sys)
    {
        JetVector taggableJets = SusyNtTools::getBTagSFJets2Lep(jets);
        return SusyNtTools::bTagSF(evt, taggableJets, evt->mcChannel, supersys_to_btagsys(sys));
    }


    double Superflow::computeLeptonEfficiencySf(const Susy::Lepton &lep, const SupersysWeight sys)
    {
        double effFactor = 1.0;
        double sf = lep.effSF;
        double delta = 0.0;

        if (lep.isEle()) {
            if (sys == SupersysWeight::ESFUP) {
                delta = (+lep.errEffSF);
            }
            else if (sys == SupersysWeight::ESFDOWN) {
                delta = (-lep.errEffSF);
            }
        }
        else if (lep.isMu()) {
            if (sys == SupersysWeight::MEFFUP) {
                delta = (+lep.errEffSF);
            }
            else if (sys == SupersysWeight::MEFFDOWN) {
                delta = (-lep.errEffSF);
            }
        }

        effFactor = (sf + delta); // ?? Seems odd.
        return effFactor;
    }

    EventFlags Superflow::computeEventFlags()
    { // stolen from Davide
        EventFlags f;
        if (m_dbg) cout << app_name << "computeEventFlags" << endl;
        int flag = nt.evt()->cutFlags[NtSys_NOM];
        const LeptonVector &bleps = m_baseLeptons;
        const JetVector     &jets = m_baseJets;
        const JetVector    &pjets = m_preJets;
        const Susy::Met      *met = m_met;
        uint run = nt.evt()->run;
        bool mc = nt.evt()->isMC;
        float mllMin(20);
        bool has2lep(bleps.size() > 1 && bleps[0] && bleps[1]);
        float mll(has2lep ? (*bleps[0] + *bleps[1]).M() : 0.0);
        const int killHfor(4); // inheriting hardcoded magic values from HforToolD3PD.cxx
        if (passGRL(flag))  f.grl = true;
        if (passLarErr(flag))  f.larErr = true;
        if (passTileErr(flag))  f.tileErr = true;
        if (passTTCVeto(flag))  f.ttcVeto = true;
        if (passGoodVtx(flag))  f.goodVtx = true;
        if (passTileTripCut(flag))  f.tileTrip = true;
        if (passLAr(flag))  f.lAr = true;
        if (!hasBadJet(jets))  f.badJet = true;
        if (passDeadRegions(pjets, met, run, mc)) f.deadRegions = true;
        if (!hasBadMuon(m_preMuons))  f.badMuon = true;
        if (!hasCosmicMuon(m_baseMuons))  f.cosmicMuon = true;
        if (nt.evt()->hfor != killHfor)  f.hfor = true;
        if (bleps.size() >= 2)  f.ge2blep = true;
        if (bleps.size() == 2)  f.eq2blep = true;

        // const LeptonVector& leptons, DilTrigLogic *dtl, float met, Event* evt
        // dtl->passDilEvtTrig(leptons, met, evt);
        // dtl->passDilTrigMatch(leptons, met, evt);

        if (mll > mllMin)  f.mllMin = true;
        return f;
    }

    void Superflow::setCountWeights(bool value) ///> public function, if set true it prints the weighted cutflow
    {
        m_countWeights = value;
    }

    void Superflow::setRunMode(SuperflowRunMode run_mode_) ///> public function
    {
        m_runMode = run_mode_;
    }

    void Superflow::setSingleEventSyst(SusyNtSys nt_syst_)
    {
        m_singleEventSyst = nt_syst_;
    }
}
