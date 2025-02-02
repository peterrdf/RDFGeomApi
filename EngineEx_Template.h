//
// Helper classes (C++ wrappers)
//
#ifndef __RDF_LTD__NAMESPACE_NAME_H
#define __RDF_LTD__NAMESPACE_NAME_H

#include    <assert.h>
#include	"engine.h"
#include    "engineinline.h"

namespace NAMESPACE_NAME
{
//## TEMPLATE: ClassForwardDeclaration
    class CLASS_NAME;
//## TEMPLATE: BeginAllClasses


    /// <summary>
    /// Provides utility methods to interact with a genetic instance of OWL class
    /// You also can use object of this class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Instance
    {
    protected:
        /// <summary>
        /// underlyed instance handle
        /// </summary>
        int64_t m_instance;

    protected:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="className"></param>
        /// <param name="instanceName"></param>
        /// <param name="instanceNameW"></param>
        /// <returns></returns>
        static int64_t Create(int64_t model, const char* className, const char* instanceName, const wchar_t* instanceNameW)
        {
            int64_t clsid = GetClassByName(model, className);
            assert(clsid != 0);

            int64_t instance = 0;
            if (instanceNameW) {
                instance = CreateInstanceW(clsid, instanceNameW);
            }
            else {
                instance = CreateInstance(clsid, instanceName);
            }

            return instance;
        }
        
        /// <summary>
        /// Constructs object that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Instance(int64_t instance, const char* cls)
        {
            m_instance = instance;
            if (cls != NULL && !::IsInstanceOfClass(instance, cls)) {
                m_instance = NULL;
            }
        }

    public:
        /// <summary>
        /// Conversion to instance handle, so the object of the class can be used anywhere where a handle required
        /// </summary>
        operator int64_t() const { return m_instance; }

        /// <summary>
        /// Get property id from property name
        /// </summary>
        int64_t GetPropertyId(const char* name, int64_t checkCardinality = -1)
        {
            int64_t model = GetModel(m_instance);

            int64_t propId = GetPropertyByName(model, name);

            if (propId) {
                int64_t clsId = GetInstanceClass(m_instance);
                int64_t minCard = 0, maxCard = 0;
                GetPropertyRestrictionsConsolidated(clsId, propId, &minCard, &maxCard);
                if (minCard < 0) {
                    propId = NULL; //property is not assigned to the class
                }
                if (checkCardinality > 0) { //chek cardinatity when set property
                    if (checkCardinality < minCard || (maxCard > 0 && checkCardinality > maxCard)) {
                        propId = NULL; //cardinality is out of range
                    }
                }
            }

            return propId;
        }

        ///<summary></summary>
        template<typename TElem> bool SetDatatypeProperty(const char* name, const TElem* values, int64_t count)
        {
            int64_t propId = GetPropertyId(name, count);
            int64_t res = ::SetDatatypeProperty(m_instance, propId, values, count);
            return (res == 0);
        }


        ///<summary>The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        template<typename TElem> const TElem*  GetDatatypeProperty(const char* name, int64_t* pCount)
        {
            int64_t propId = GetPropertyId(name);

            TElem* values = NULL;
            int64_t count = 0;
            int64_t res = ::GetDatatypeProperty(m_instance, propId, (void**)&values, &count);
            if (res != 0) {
                count = 0;
            }

            if (pCount) {
                *pCount = count;
            }

            if (count > 0) {
                return values;
            }
            else {
                return NULL;
            }
        }


        ///<summary></summary>
        template<class TInstance> bool SetObjectProperty(const char* name, const TInstance* instances, int64_t count)
        {
            int64_t propId = GetPropertyId(name, count);
            int64_t res = ::SetObjectProperty(m_instance, propId, (int64_t*)instances, count);
            return(res == 0);
        }

        ///<summary>The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        template<class TInstance> const TInstance* GetObjectProperty(const char* name, int64_t* pCount)
        {
            int64_t propId = GetPropertyId(name);

            int64_t* values = NULL;
            int64_t count = 0;
            int64_t res = ::GetObjectProperty(m_instance, propId, &values, &count);
            if (res != 0) {
                count = 0;            
            }

            if (pCount) {
                *pCount = count;
            }

            if (count > 0) {
                return (TInstance*)values;
            }
            else {
                return NULL;
            }
        }
    };

    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="TList"></typeparam>
    /// <typeparam name="TElem"></typeparam>
    /// <param name="values"></param>
    /// <param name="arr"></param>
    /// <param name="count"></param>
    template <typename TList, typename TElem> void ListToArray(const TList& values, TElem** arr, int64_t* count)
    {
        *count = 0;
        for (const TElem v : values) {
            (*count)++;
        }

        if (*count) {
            *arr = new TElem[*count];
            int64_t i = 0;
            for (const TElem v : values) {
                (*arr)[i++] = v;
            }
        }
        else {
            *arr = NULL;
        }
    }


//## TEMPLATE: BeginWrapperClass

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class CLASS_NAME
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CLASS_NAME : public /*BASE CLASS*/Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CLASS_NAME and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CLASS_NAME Create(int64_t model, const char* name=NULL) { return CLASS_NAME(Instance::Create(model, "CLASS_NAME", name, NULL), "CLASS_NAME");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CLASS_NAME CreateW(int64_t model, const wchar_t* name = NULL) { return CLASS_NAME(Instance::Create(model, "CLASS_NAME", NULL, name), "CLASS_NAME"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        CLASS_NAME(int64_t instance = NULL)
            : /*BASE CLASS*/Instance(instance, "CLASS_NAME")
        {}

    protected:
        CLASS_NAME(int64_t instance, const char* checkClassName)
            : /*BASE CLASS*/Instance(instance, checkClassName)
        {}
//## TEMPLATE StartPropertiesBlock

    public:
       //
       // Properties with known cardinality restrictions to PROPERTIES_OF_CLASS
       //

//## TEMPLATE: SetDataProperty
        ///<summary>Sets value of PROPERTY_NAME</summary>
        bool set_PROPERTY_NAME(double value) { return SetDatatypeProperty ("PROPERTY_NAME", &value, 1); }
//## TEMPLATE SetDataArrayProperty
        ///<summary>Sets values of PROPERTY_NAME. OWL cardinality CARDINALITY_MIN..CARDINALITY_MAX</summary>
        bool set_PROPERTY_NAME(const double* values, int64_t count) { return SetDatatypeProperty ("PROPERTY_NAME", values, count); }
        ///<summary>Sets values of PROPERTY_NAME from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality CARDINALITY_MIN..CARDINALITY_MAX</summary>
        template <typename TList> bool set_PROPERTY_NAME(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_PROPERTY_NAME(arr, count); if (arr) delete[] arr; return ok; }
//## TEMPLATE GetDataProperty
        ///<summary>Gets a value of PROPERTY_NAME, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_PROPERTY_NAME() { return GetDatatypeProperty<double>("PROPERTY_NAME", NULL); }
//## TEMPLATE GetDataArrayProperty
        ///<summary>Gets values array of PROPERTY_NAME. OWL cardinality CARDINALITY_MIN..CARDINALITY_MAX. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_PROPERTY_NAMEasType(int64_t* pCount) { return GetDatatypeProperty<double>("PROPERTY_NAME", pCount); }
//## TEMPLATE: SetObjectProperty
        ///<summary>Sets relationship from this instance to an instance of Instance</summary>
        bool set_PROPERTY_NAME(const Instance& instance) { return SetObjectProperty<Instance>("PROPERTY_NAME", &instance, 1); }
//## TEMPLATE SetObjectArrayProperty
        ///<summary>Sets relationships from this instance to an array of Instance. OWL cardinality CARDINALITY_MIN..CARDINALITY_MAX</summary>
        bool set_PROPERTY_NAME(const Instance* instances, int64_t count) { return SetObjectProperty<Instance>("PROPERTY_NAME", instances, count); }
//## TEMPLATE GetObjectProperty
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Instance* get_PROPERTY_NAMEasTYPe() { return GetObjectProperty<Instance>("PROPERTY_NAME", NULL); }
//## TEMPLATE GetObjectArrayProperty
        ///<summary>Get an array of related instances. OWL cardinality CARDINALITY_MIN..CARDINALITY_MAX. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Instance* get_PROPERTY_NAMEasTYPE(int64_t* pCount) { return GetObjectProperty<Instance>("PROPERTY_NAME", pCount); }
//## TEMPLATE GetObjectArrayPropertyInt64
        ///<summary>Get an array of related instance handles. OWL cardinality CARDINALITY_MIN..CARDINALITY_MAX. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_PROPERTY_NAME_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("PROPERTY_NAME", pCount); }
//## TEMPLATE: EndWrapperClass
    };
//## TEMPLATE: EndFile template part
}

#endif
