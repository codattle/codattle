package com.codattle.annotation.processing.generator

import com.codattle.annotation.processing.annotation.GenerateDaoModelBuilder
import com.google.auto.service.AutoService
import com.squareup.kotlinpoet.*
import com.squareup.kotlinpoet.ParameterizedTypeName.Companion.parameterizedBy
import java.io.File
import javax.annotation.processing.*
import javax.lang.model.SourceVersion
import javax.lang.model.element.ElementKind
import javax.lang.model.element.ExecutableElement
import javax.lang.model.element.TypeElement

@AutoService(Processor::class)
@SupportedSourceVersion(SourceVersion.RELEASE_8)
@SupportedOptions(DaoModelBuilderGenerator.KAPT_KOTLIN_GENERATED_OPTION_NAME)
class DaoModelBuilderGenerator : AbstractProcessor() {

    companion object {
        const val KAPT_KOTLIN_GENERATED_OPTION_NAME = "kapt.kotlin.generated"
    }

    private data class Parameter(val name: String, val type: TypeName, val nonNullable: Boolean)

    override fun getSupportedAnnotationTypes(): MutableSet<String> {
        return mutableSetOf(GenerateDaoModelBuilder::class.java.name)
    }

    override fun getSupportedSourceVersion(): SourceVersion {
        return SourceVersion.latest()
    }

    override fun process(set: MutableSet<out TypeElement>?, roundEnvironment: RoundEnvironment?): Boolean {
        roundEnvironment?.getElementsAnnotatedWith(GenerateDaoModelBuilder::class.java)?.forEach { element ->
            generateDaoModelBuilderForDaoModel(element as TypeElement)
        }
        return true
    }

    private fun generateDaoModelBuilderForDaoModel(modelClass: TypeElement) {
        val kaptKotlinGeneratedDir = processingEnv.options[KAPT_KOTLIN_GENERATED_OPTION_NAME]
        val packageName = processingEnv.elementUtils.getPackageOf(modelClass).toString()
        val modelClassName = modelClass.simpleName.toString()
        val builderClassName = "${modelClassName}Builder"
        val builderClass = ClassName(packageName, builderClassName)
        val parameters = getFirstConstructor(modelClass).parameters.map {
            Parameter(
                    name = it.simpleName.toString(),
                    type = mapJavaTypeToKotlinType(it.asType().asTypeName()).copy(nullable = true),
                    nonNullable = it.getAnnotation(org.jetbrains.annotations.NotNull::class.java) != null)
        }

        val fileSpec = FileSpec.builder(packageName, builderClassName)
                .addType(TypeSpec.classBuilder(builderClassName)
                        .addModifiers(KModifier.DATA)
                        .addSuperinterface(ClassName("com.codattle.core.dao.common", "DaoModelBuilder")
                                .parameterizedBy(ClassName(packageName, modelClassName)))
                        .primaryConstructor(generateConstructor(parameters))
                        .addType(TypeSpec.companionObjectBuilder()
                                .addFunction(generateFromModelFunction(modelClass, builderClassName, parameters))
                                .addFunction(generateWithoutDefaultFunction(builderClassName, parameters))
                                .build())
                        .addFunction(generateBuildFunction(modelClass, parameters))
                        .addProperties(generateProperties(parameters))
                        .addFunctions(generateChainSetters(builderClass, parameters))
                        .build())
                .build()

        fileSpec.writeTo(File(kaptKotlinGeneratedDir, "$builderClassName.kt"))
    }

    private fun getFirstConstructor(typeElement: TypeElement): ExecutableElement {
        return typeElement.enclosedElements.first { it.kind === ElementKind.CONSTRUCTOR } as ExecutableElement
    }

    private fun generateConstructor(parameters: List<Parameter>): FunSpec {
        val constructorSpec = FunSpec.constructorBuilder().addModifiers(KModifier.PRIVATE)
        parameters.forEach {
            constructorSpec.addParameter(ParameterSpec.builder(it.name, it.type).build())
        }

        return constructorSpec.build()
    }

    private fun generateProperties(parameters: List<Parameter>): List<PropertySpec> {
        return parameters.map {
            val propertySpec = PropertySpec.builder(it.name, it.type).initializer(it.name).mutable(true)
            if (it.name in listOf("id", "creationDate")) {
                propertySpec.addModifiers(KModifier.OVERRIDE)
            }
            propertySpec.build()
        }
    }

    private fun generateChainSetters(builderClass: ClassName, parameters: List<Parameter>): List<FunSpec> {
        return parameters.map {
            FunSpec.builder(it.name)
                    .addParameter(it.name, it.type)
                    .returns(builderClass)
                    .addStatement("this.${it.name} = ${it.name}")
                    .addStatement("return this")
                    .build()
        }
    }

    private fun generateFromModelFunction(modelClass: TypeElement, builderClassName: String, parameters: List<Parameter>): FunSpec {
        return FunSpec.builder("fromModel")
                .addParameter("model", modelClass.asClassName())
                .returns(ClassName("", builderClassName))
                .addCode("return $builderClassName(\n")
                .addCode(parameters.map { it.name }.joinToString(",\n") { "  $it = model.$it" })
                .addCode("\n)")
                .build()
    }

    private fun generateWithoutDefaultFunction(builderClassName: String, parameters: List<Parameter>): FunSpec {
        val functionSpec = FunSpec.builder("withoutDefault")

        parameters.forEach {
            functionSpec.addParameter(ParameterSpec.builder(it.name, it.type).defaultValue("null").build())
        }

        return functionSpec
                .returns(ClassName("", builderClassName))
                .addCode("return $builderClassName(\n")
                .addCode(parameters.map { it.name }.joinToString(",\n") { "  $it = $it" })
                .addCode("\n)")
                .build()
    }

    private fun generateBuildFunction(modelClass: TypeElement, parameters: List<Parameter>): FunSpec {
        val functionSpec = FunSpec.builder("build")
                .addModifiers(KModifier.OVERRIDE)
                .returns(modelClass.asClassName())
                .addCode("return %T(\n", modelClass)

        parameters.forEachIndexed { index, parameter ->
            if (parameter.nonNullable) {
                functionSpec.addCode(" ${parameter.name} = ${parameter.name} ?: throw IllegalStateException(%S)", "Field ${parameter.name} cannot be null while building")
            } else {
                functionSpec.addCode(" ${parameter.name} = ${parameter.name}")
            }
            if (index != parameters.lastIndex) {
                functionSpec.addCode(",\n")
            }
        }

        functionSpec.addCode("\n)")

        return functionSpec.build()
    }

    private fun mapJavaTypeToKotlinType(javaType: TypeName): TypeName {
        val javaTypeName = javaType.toString()
        if (javaTypeName == "java.lang.String") {
            return String::class.asTypeName()
        }
        if (javaTypeName == "java.lang.Integer") {
            return Int::class.asTypeName()
        }
        if (javaType is ParameterizedTypeName && javaType.rawType == ClassName("java.util", "List")) {
            return List::class.asClassName().parameterizedBy(mapJavaTypeToKotlinType(javaType.typeArguments[0]))
        }
        if (javaType is ParameterizedTypeName && javaType.rawType == ClassName("java.util", "Set")) {
            return Set::class.asClassName().parameterizedBy(mapJavaTypeToKotlinType(javaType.typeArguments[0]))
        }
        return javaType
    }
}
