#pragma once

namespace usdi {

class Context
{
public:
    Context();
    virtual ~Context();

    bool                valid() const;
    void                initialize();
    bool                createStage(const char *identifier);
    bool                open(const char *path);
    bool                save() const;
    // path must *not* be same as identifier (parameter of createStage() or open())
    bool                saveAs(const char *path) const;

    const ImportSettings&   getImportSettings() const;
    void                    setImportSettings(const ImportSettings& v);
    const ExportSettings&   getExportSettings() const;
    void                    setExportSettings(const ExportSettings& v);

    UsdStageRefPtr      getUsdStage() const;
    Schema*             getRoot() const;
    int                 getNumSchemas() const;
    Schema*             getSchema(int i) const;
    int                 getNumMasters() const;
    Schema*             getMaster(int i) const;
    Schema*             findSchema(const char *path) const;

    // SchemaType: Xform, Camera, Mesh, etc
    template<class SchemaType>
    SchemaType*         createSchema(Schema *parent, const char *name);
    Schema*             createSchema(Schema *parent, const UsdPrim& prim);
    Schema*             createSchemaRecursive(Schema *parent, UsdPrim prim);
    Schema*             createInstanceSchema(Schema *parent, Schema *master, const std::string& path, UsdPrim prim);
    Schema*             createInstanceSchemaRecursive(Schema *parent, UsdPrim prim);
    Schema*             createOverride(const char *prim_path);
    void                flatten();

    void                beginEdit(const UsdEditTarget& t);
    void                endEdit();

    void                rebuildSchemaTree();
    int                 generateID();
    void                notifyForceUpdate();
    void                updateAllSamples(Time t);

    using precomputeNormalsCallback = std::function<void(Mesh*, bool)>;
    void                precomputeNormalsAll(bool gen_tangents, bool overwrite, const precomputeNormalsCallback& cb);

private:
    void    addSchema(Schema *schema);
    void    applyImportConfig();

private:
    using SchemaPtr = std::unique_ptr<Schema>;
    using Schemas = std::vector<SchemaPtr>;
    using Masters = std::vector<Schema*>;
    using EditTargets = std::vector<UsdEditTarget>;

    UsdStageRefPtr  m_stage;
    Schemas         m_schemas;
    Schema*         m_root = nullptr;
    Masters         m_masters;

    ImportSettings  m_import_settings;
    ExportSettings  m_export_settings;

    int             m_id_seed = 0;
    double          m_start_time = 0.0;
    double          m_end_time = 0.0;
    EditTargets     m_edit_targets;
};

} // namespace usdi
